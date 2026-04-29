using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace luo.dangxiao.printer.Virtual
{
    /// <summary>
    /// Simulated card printer implementation for development and testing.
    /// </summary>
    public sealed class VirtualCardPrinter : CardPrinterBase
    {
        private const string DefaultPrinterId = "virtual-printer-001";
        private readonly Dictionary<string, PrinterInfo> _printers = new(StringComparer.OrdinalIgnoreCase)
        {
            [DefaultPrinterId] = new PrinterInfo
            {
                ProviderName = "Virtual",
                Id = DefaultPrinterId,
                Name = "Virtual Card Printer",
                Model = "Virtual-Simulator",
                Address = "memory://virtual-printer-001",
                FirmwareVersion = "sim-1.0.0",
                SerialNumber = "VIRTUAL-0001",
                Status = PrinterStatus.Ready,
                LastSeen = DateTime.Now
            }
        };

        private readonly Dictionary<string, CardPositionState> _positions = new(StringComparer.OrdinalIgnoreCase)
        {
            [DefaultPrinterId] = CardPositionState.OutOfPrinter
        };

        /// <inheritdoc />
        public override string ProviderName => "Virtual";

        /// <inheritdoc />
        public override Task<IReadOnlyList<PrinterInfo>> DiscoverPrintersAsync()
        {
            foreach (var printer in _printers.Values)
            {
                printer.LastSeen = DateTime.Now;
            }

            return Task.FromResult<IReadOnlyList<PrinterInfo>>(_printers.Values.ToList());
        }

        /// <inheritdoc />
        public override Task<bool> ConnectAsync(string printerId)
        {
            if (!_printers.TryGetValue(printerId, out var printer))
                return Task.FromResult(false);

            printer.IsConnected = true;
            printer.Status = PrinterStatus.Ready;
            printer.LastSeen = DateTime.Now;
            return Task.FromResult(true);
        }

        /// <inheritdoc />
        public override Task<bool> DisconnectAsync(string printerId)
        {
            if (!_printers.TryGetValue(printerId, out var printer))
                return Task.FromResult(false);

            printer.IsConnected = false;
            printer.Status = PrinterStatus.Offline;
            return Task.FromResult(true);
        }

        /// <inheritdoc />
        public override async Task<bool> SendPrintJobAsync(string printerId, PrintJob job)
        {
            if (!_printers.TryGetValue(printerId, out var printer) || !printer.IsConnected)
            {
                job.Status = JobStatus.Failed;
                job.ErrorMessage = "Virtual printer is not connected.";
                return false;
            }

            job.Status = JobStatus.Processing;
            job.StartedAt = DateTime.Now;

            await MoveCardAsync(printerId, CardMoveCommand.MoveToPrepare);
            await Task.Delay(1000);
            await MoveCardAsync(printerId, CardMoveCommand.MoveToFront);
            await Task.Delay(1000);

            printer.Status = PrinterStatus.Printing;
            job.Status = JobStatus.Printing;
            _positions[printerId] = CardPositionState.PrintStation;

            await Task.Delay(2000);

            printer.Status = PrinterStatus.Ready;
            job.Status = JobStatus.Completed;
            job.CompletedAt = DateTime.Now;
            _positions[printerId] = CardPositionState.PrintCompleted;
            return true;
        }

        /// <inheritdoc />
        public override Task<PrinterStatus> GetPrinterStatusAsync(string printerId)
        {
            if (!_printers.TryGetValue(printerId, out var printer))
                return Task.FromResult(PrinterStatus.Offline);

            return Task.FromResult(printer.Status);
        }

        /// <inheritdoc />
        public override Task<byte[]> GetPrinterInfoAsync(string printerId)
        {
            if (!_printers.TryGetValue(printerId, out var printer))
                return Task.FromResult(Array.Empty<byte>());

            var payload = new StringBuilder()
                .AppendLine($"Provider: {printer.ProviderName}")
                .AppendLine($"Name: {printer.Name}")
                .AppendLine($"Model: {printer.Model}")
                .AppendLine($"Address: {printer.Address}")
                .AppendLine($"Status: {printer.Status}")
                .AppendLine($"CardPosition: {_positions[printerId]}")
                .ToString();

            return Task.FromResult(Encoding.UTF8.GetBytes(payload));
        }

        /// <inheritdoc />
        public override CardPrintSessionBase BeginPrintSession(string printerId)
        {
            if (!_printers.TryGetValue(printerId, out var printer) || !printer.IsConnected)
                throw new InvalidOperationException("Virtual printer is not connected.");

            return new VirtualPrintSession(this, printerId);
        }

        /// <inheritdoc />
        public override Task<bool> MoveCardAsync(string printerId, CardMoveCommand command)
        {
            if (!_positions.ContainsKey(printerId))
                return Task.FromResult(false);

            _positions[printerId] = command switch
            {
                CardMoveCommand.MoveToHopper => CardPositionState.OutOfPrinter,
                CardMoveCommand.MoveToContact => CardPositionState.Contact,
                CardMoveCommand.MoveToContactless => CardPositionState.Contactless,
                CardMoveCommand.MoveToStandbyBack => CardPositionState.BackStandby,
                CardMoveCommand.MoveToFlipper => CardPositionState.Flipper,
                CardMoveCommand.MoveToRejectBoxFront => CardPositionState.RejectBox,
                CardMoveCommand.MoveToRejectBoxDown => CardPositionState.RejectBox,
                CardMoveCommand.MoveToStandbyFront => CardPositionState.FrontStandby,
                CardMoveCommand.MoveToFront => CardPositionState.Front,
                CardMoveCommand.MoveToPrepare => CardPositionState.Prepare,
                CardMoveCommand.MoveToIndentPrinter => CardPositionState.IndentPrinter,
                CardMoveCommand.MoveToStandbyDown => CardPositionState.DownStandby,
                CardMoveCommand.MoveToStorage => CardPositionState.Storage,
                CardMoveCommand.MoveFromStorageToPrepare => CardPositionState.Prepare,
                _ => CardPositionState.Unknown
            };

            return Task.Delay(1000).ContinueWith(p => true);
        }

        /// <inheritdoc />
        public override Task<bool> ResetPrinterAsync(string printerId, bool hardReset = false)
        {
            if (!_printers.TryGetValue(printerId, out var printer))
                return Task.FromResult(false);

            printer.Status = PrinterStatus.Ready;
            _positions[printerId] = CardPositionState.OutOfPrinter;
            return Task.Delay(1000).ContinueWith(p => true);
        }

        /// <inheritdoc />
        public override Task<CardPositionState> GetCardPositionAsync(string printerId)
        {
            if (!_positions.TryGetValue(printerId, out var position))
                return Task.FromResult(CardPositionState.Unknown);

            return Task.FromResult(position);
        }

        private sealed class VirtualPrintSession : CardPrintSessionBase
        {
            private readonly VirtualCardPrinter _printer;
            private readonly string _printerId;
            private bool _disposed;
            private bool _pageStarted;

            public VirtualPrintSession(VirtualCardPrinter printer, string printerId)
            {
                _printer = printer;
                _printerId = printerId;
            }

            public override CardPrintSessionBase BeginPage()
            {
                ThrowIfDisposed();
                _pageStarted = true;
                return this;
            }

            public override CardPrintSessionBase EndPage()
            {
                ThrowIfDisposed();
                _pageStarted = false;
                return this;
            }

            public override CardPrintSessionBase PrintImage(int x, int y, int width, int height, string imagePath)
            {
                ThrowIfDisposed();
                EnsurePageStarted();
                _printer._positions[_printerId] = CardPositionState.PrintStation;
                return this;
            }

            public override CardPrintSessionBase PrintText(int x, int y, string text, string fontName = "SimSun", int fontSize = 10, int fontWeight = 400, CardFontAttribute attributes = CardFontAttribute.Normal, uint textColor = 0, bool transparentBack = true)
            {
                ThrowIfDisposed();
                EnsurePageStarted();
                _printer._positions[_printerId] = CardPositionState.PrintStation;
                return this;
            }

            public override void Cancel()
            {
                if (_disposed)
                    return;

                _printer._positions[_printerId] = CardPositionState.OutOfPrinter;
                _disposed = true;
            }

            public override void Dispose()
            {
                if (_disposed)
                    return;

                _printer._positions[_printerId] = CardPositionState.PrintCompleted;
                _disposed = true;
            }

            private void EnsurePageStarted()
            {
                if (!_pageStarted)
                    throw new InvalidOperationException("Must call BeginPage first.");
            }

            private void ThrowIfDisposed()
            {
                if (_disposed)
                    throw new ObjectDisposedException(nameof(VirtualPrintSession));
            }
        }
    }
}
