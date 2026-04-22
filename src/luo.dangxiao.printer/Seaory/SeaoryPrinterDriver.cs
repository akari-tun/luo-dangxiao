using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using luo.dangxiao.printer.Seaory.Native;

namespace luo.dangxiao.printer.Seaory
{
    /// <summary>
    /// High-level driver for Seaory card printers using the native SDK.
    /// Implements the common card printer abstraction for Seaory hardware.
    /// </summary>
    public class SeaoryPrinterDriver : CardPrinterBase
    {
        private const string Provider = "SeaoryS22K";
        private const int InfoBufferSize = 256;
        private const int MaxTrackBufferSize = 256;
        
        private readonly Dictionary<string, PrinterInfo> _connectedPrinters = new();
        private readonly Dictionary<string, IntPtr> _printContexts = new();
        
        private static readonly object _sdkLock = new();

        /// <inheritdoc />
        public override string ProviderName => Provider;

        public event EventHandler<PrinterStatusEventArgs>? StatusChanged;
        public event EventHandler<PrintJobEventArgs>? JobStatusChanged;

        #region SDK Initialization

        public string GetSdkVersion()
        {
            var buffer = new byte[InfoBufferSize];
            SeaorySdk.SOY_PR_SdkVersionA(buffer);
            return Encoding.ASCII.GetString(buffer).TrimEnd('\0');
        }

        public void SetLogLevel(bool enabled)
        {
            SeaorySdk.SOY_PR_SetLogLevel(enabled ? 1u : 0u);
        }

        #endregion

        #region Printer Discovery

        /// <inheritdoc />
        public override Task<IReadOnlyList<PrinterInfo>> DiscoverPrintersAsync()
        {
            return Task.Run<IReadOnlyList<PrinterInfo>>(() =>
            {
                var printers = new List<PrinterInfo>();

                lock (_sdkLock)
                {
                    if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
                    {
                        var windowsPrinters = DiscoverWindowsPrinters();
                        printers.AddRange(windowsPrinters);
                    }
                    else if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
                    {
                        var usbPrinters = DiscoverLinuxUsbPrinters();
                        printers.AddRange(usbPrinters);
                    }
                }

                return printers;
            });
        }

        private IEnumerable<PrinterInfo> DiscoverWindowsPrinters()
        {
            var printers = new List<PrinterInfo>();

            var commonPrinterNames = new[]
            {
                "Seaory S22K"
            };

            foreach (var printerName in commonPrinterNames)
            {
                var printer = TryConnectToPrinter(printerName);
                if (printer != null)
                {
                    printers.Add(printer);
                    _connectedPrinters.Remove(printerName);
                }
            }

            return printers;
        }

        private IEnumerable<PrinterInfo> DiscoverLinuxUsbPrinters()
        {
            var printers = new List<PrinterInfo>();

            for (int i = 0; i < 10; i++)
            {
                var devicePath = $"/dev/usb/lp{i}";
                
                if (!File.Exists(devicePath))
                    continue;

                var printer = TryConnectToPrinter(devicePath);
                if (printer != null)
                {
                    printers.Add(printer);
                    _connectedPrinters.Remove(devicePath);
                }
            }

            return printers;
        }

        private PrinterInfo? TryConnectToPrinter(string port)
        {
            try
            {
                var status = GetStatus(port);
                if (status == PrinterStatus.Offline)
                    return null;

                var model = GetPrinterInfo(port, PrinterInfoType.ModelName);
                var firmware = GetPrinterInfo(port, PrinterInfoType.FirmwareVersion);
                var serial = GetPrinterInfo(port, PrinterInfoType.SerialNumber);

                return new PrinterInfo
                {
                    ProviderName = Provider,
                    Id = port,
                    Name = string.IsNullOrEmpty(model) ? $"Seaory Printer ({port})" : model,
                    Model = model,
                    Address = port,
                    Status = status,
                    FirmwareVersion = firmware,
                    SerialNumber = serial,
                    LastSeen = DateTime.Now,
                    IsConnected = false
                };
            }
            catch
            {
                return null;
            }
        }

        #endregion

        #region Connection Management

        /// <inheritdoc />
        public override Task<bool> ConnectAsync(string printerId)
        {
            return Task.Run(() =>
            {
                lock (_sdkLock)
                {
                    if (_connectedPrinters.ContainsKey(printerId))
                        return true;

                    uint status = 0;
                    var result = SeaorySdk.SOY_PR_GetPrinterStatusA(printerId, ref status);

                    if (result != 0)
                        return false;

                    var printer = TryConnectToPrinter(printerId);
                    if (printer == null)
                        return false;

                    printer.IsConnected = true;
                    _connectedPrinters[printerId] = printer;
                    
                    return true;
                }
            });
        }

        /// <inheritdoc />
        public override Task<bool> DisconnectAsync(string printerId)
        {
            return Task.Run(() =>
            {
                lock (_sdkLock)
                {
                    if (_printContexts.TryGetValue(printerId, out var context) && context != IntPtr.Zero)
                    {
                        SeaorySdk.SOY_PR_EndPrinting2(context, true);
                        _printContexts.Remove(printerId);
                    }

                    return _connectedPrinters.Remove(printerId);
                }
            });
        }

        #endregion

        #region Printer Status

        /// <inheritdoc />
        public override Task<PrinterStatus> GetPrinterStatusAsync(string printerId)
        {
            return Task.Run(() => GetStatus(printerId));
        }

        private PrinterStatus GetStatus(string printerId)
        {
            uint status = 0;
            var result = SeaorySdk.SOY_PR_GetPrinterStatusA(printerId, ref status);

            if (result == ErrorCode.DeviceNotConnected)
                return PrinterStatus.Offline;

            if (result != 0)
                return PrinterStatus.Error;

            return ConvertStatus(status);
        }

        private static PrinterStatus ConvertStatus(uint sdkStatus)
        {
            return sdkStatus switch
            {
                0 => PrinterStatus.Ready,
                ErrorCode.DeviceBusy => PrinterStatus.Printing,
                ErrorCode.DeviceNotConnected => PrinterStatus.Offline,
                ErrorCode.CoverOpen => PrinterStatus.Error,
                ErrorCode.RibbonOut => PrinterStatus.Error,
                ErrorCode.RibbonMissing => PrinterStatus.Error,
                ErrorCode.CardOut => PrinterStatus.Error,
                _ when sdkStatus >= 0x00010010 && sdkStatus <= 0x0001001F => PrinterStatus.Error,
                _ => PrinterStatus.Online
            };
        }

        #endregion

        #region Printer Info

        /// <inheritdoc />
        public override Task<byte[]> GetPrinterInfoAsync(string printerId)
        {
            return Task.Run(() =>
            {
                var info = new Dictionary<string, string>
                {
                    ["Model"] = GetPrinterInfo(printerId, PrinterInfoType.ModelName),
                    ["Firmware"] = GetPrinterInfo(printerId, PrinterInfoType.FirmwareVersion),
                    ["Serial"] = GetPrinterInfo(printerId, PrinterInfoType.SerialNumber),
                    ["RibbonType"] = GetPrinterInfo(printerId, PrinterInfoType.RibbonType),
                    ["RibbonCount"] = GetPrinterInfo(printerId, PrinterInfoType.RibbonCount),
                    ["PrintCount"] = GetPrinterInfo(printerId, PrinterInfoType.PrintCount),
                    ["CardPosition"] = GetPrinterInfo(printerId, PrinterInfoType.CardPosition),
                    ["MacAddress"] = GetPrinterInfo(printerId, PrinterInfoType.MacAddress)
                };

                var sb = new StringBuilder();
                foreach (var kvp in info)
                {
                    sb.AppendLine($"{kvp.Key}: {kvp.Value}");
                }

                return Encoding.UTF8.GetBytes(sb.ToString());
            });
        }

        public string GetPrinterInfo(string printerId, int infoType)
        {
            var buffer = new byte[InfoBufferSize];
            var result = SeaorySdk.SOY_PR_GetPrinterInfoA(printerId, infoType, buffer);

            if (result != 0)
                return string.Empty;

            return Encoding.ASCII.GetString(buffer).TrimEnd('\0');
        }

        public int GetPrinterConfig(string printerId, uint configType)
        {
            int value = 0;
            var result = SeaorySdk.SOY_PR_GetPrinterConfigA(printerId, configType, ref value);
            return result == 0 ? value : -1;
        }

        public uint SetPrinterConfig(string printerId, uint configType, int value)
        {
            return SeaorySdk.SOY_PR_SetPrinterConfigA(printerId, configType, value);
        }

        #endregion

        #region Print Operations

        /// <inheritdoc />
        public override Task<bool> SendPrintJobAsync(string printerId, PrintJob job)
        {
            return Task.Run(() =>
            {
                lock (_sdkLock)
                {
                    try
                    {
                        IntPtr context = IntPtr.Zero;
                        var docPropPtr = IntPtr.Zero;

                        try
                        {
                            var docProp = SeaoryDocProp.Create();
                            docProp.byPrintSide = 1;
                            docProp.byAutoDetectRibbon = 1;
                            docProp.byCardInOutByDev = 1;

                            docPropPtr = Marshal.AllocHGlobal(Marshal.SizeOf<SeaoryDocProp>());
                            Marshal.StructureToPtr(docProp, docPropPtr, false);

                            var result = SeaorySdk.SOY_PR_StartPrinting2A(printerId, docPropPtr, ref context);
                            if (result != 0)
                            {
                                job.Status = JobStatus.Failed;
                                job.ErrorMessage = GetErrorDescription(result);
                                return false;
                            }

                            _printContexts[printerId] = context;

                            result = SeaorySdk.SOY_PR_StartPage2(context);
                            if (result != 0)
                            {
                                job.Status = JobStatus.Failed;
                                job.ErrorMessage = GetErrorDescription(result);
                                SeaorySdk.SOY_PR_EndPrinting2(context, true);
                                return false;
                            }

                            job.Status = JobStatus.Printing;
                            job.StartedAt = DateTime.Now;

                            if (job.PrintData != null && job.PrintData.Length > 0)
                            {
                                var tempFile = Path.Combine(Path.GetTempPath(), $"print_{job.Id}.bmp");
                                File.WriteAllBytes(tempFile, job.PrintData);

                                result = SeaorySdk.SOY_PR_PrintImage2A(context, 0, 0, 0, 0, tempFile);

                                if (File.Exists(tempFile))
                                    File.Delete(tempFile);

                                if (result != 0)
                                {
                                    job.Status = JobStatus.Failed;
                                    job.ErrorMessage = GetErrorDescription(result);
                                    SeaorySdk.SOY_PR_EndPage2(context);
                                    SeaorySdk.SOY_PR_EndPrinting2(context, true);
                                    return false;
                                }
                            }

                            result = SeaorySdk.SOY_PR_EndPage2(context);
                            if (result != 0)
                            {
                                job.Status = JobStatus.Failed;
                                job.ErrorMessage = GetErrorDescription(result);
                                SeaorySdk.SOY_PR_EndPrinting2(context, true);
                                return false;
                            }

                            result = SeaorySdk.SOY_PR_EndPrinting2(context, false);
                            if (result != 0)
                            {
                                job.Status = JobStatus.Failed;
                                job.ErrorMessage = GetErrorDescription(result);
                                return false;
                            }

                            job.Status = JobStatus.Completed;
                            job.CompletedAt = DateTime.Now;
                            return true;
                        }
                        finally
                        {
                            if (docPropPtr != IntPtr.Zero)
                                Marshal.FreeHGlobal(docPropPtr);
                        }
                    }
                    catch (Exception ex)
                    {
                        job.Status = JobStatus.Failed;
                        job.ErrorMessage = ex.Message;
                        return false;
                    }
                }
            });
        }

        #endregion

        #region Simple Print API

        /// <inheritdoc />
        public override CardPrintSessionBase BeginPrintSession(string printerId)
        {
            return BeginPrintSession(printerId, null);
        }

        /// <summary>
        /// Starts a Seaory-specific print session.
        /// </summary>
        /// <param name="printerId">The printer identifier.</param>
        /// <param name="docProp">Optional native document properties.</param>
        /// <returns>A Seaory print session.</returns>
        public PrintSession BeginPrintSession(string printerId, SeaoryDocProp? docProp = null)
        {
            lock (_sdkLock)
            {
                var prop = docProp ?? SeaoryDocProp.Create();
                var propPtr = Marshal.AllocHGlobal(Marshal.SizeOf<SeaoryDocProp>());
                Marshal.StructureToPtr(prop, propPtr, false);

                IntPtr context = IntPtr.Zero;
                var result = SeaorySdk.SOY_PR_StartPrinting2A(printerId, propPtr, ref context);
                Marshal.FreeHGlobal(propPtr);

                if (result != 0)
                    throw new SeaoryPrinterException(result, GetErrorDescription(result));

                return new PrintSession(this, printerId, context);
            }
        }

        internal void EndPrintSession(IntPtr context, bool cancel)
        {
            SeaorySdk.SOY_PR_EndPrinting2(context, cancel);
        }

        internal uint StartPage(IntPtr context)
        {
            return SeaorySdk.SOY_PR_StartPage2(context);
        }

        internal uint EndPage(IntPtr context)
        {
            return SeaorySdk.SOY_PR_EndPage2(context);
        }

        internal uint PrintImage(IntPtr context, int x, int y, int width, int height, string imagePath)
        {
            return SeaorySdk.SOY_PR_PrintImage2A(context, x, y, width, height, imagePath);
        }

        internal uint PrintText(IntPtr context, int x, int y, string text, string fontName, 
            int fontSize, int fontWeight, FontAttribute attributes, uint textColor, bool transparentBack)
        {
            return SeaorySdk.SOY_PR_PrintText2A(context, x, y, text, fontName, fontSize, fontWeight,
                (byte)attributes, 0, textColor, transparentBack);
        }

        #endregion

        #region Card Operations

        /// <inheritdoc />
        public override Task<bool> MoveCardAsync(string printerId, CardMoveCommand command)
        {
            return Task.Run(() => MoveCard(printerId, MapMoveCommand(command)) == ErrorCode.Success);
        }

        /// <inheritdoc />
        public override Task<bool> ResetPrinterAsync(string printerId, bool hardReset = false)
        {
            return Task.Run(() => ResetPrinter(printerId, hardReset) == ErrorCode.Success);
        }

        /// <inheritdoc />
        public override Task<CardPositionState> GetCardPositionAsync(string printerId)
        {
            return Task.Run(() => ConvertCardPosition(GetCardPosition(printerId)));
        }

        public uint MoveCard(string printerId, uint command)
        {
            return SeaorySdk.SOY_PR_ExecCommandA(printerId, command);
        }

        public uint EjectCard(string printerId)
        {
            return SeaorySdk.SOY_PR_ExecCommandA(printerId, PrinterCommand.MoveCardToHopper);
        }

        public uint ResetPrinter(string printerId, bool hardReset = false)
        {
            var command = hardReset ? PrinterCommand.ResetPrinterHard : PrinterCommand.ResetPrinterJam;
            return SeaorySdk.SOY_PR_ExecCommandA(printerId, command);
        }

        public string GetCardPosition(string printerId)
        {
            return GetPrinterInfo(printerId, PrinterInfoType.CardPosition);
        }

        private static uint MapMoveCommand(CardMoveCommand command)
        {
            return command switch
            {
                CardMoveCommand.MoveToHopper => PrinterCommand.MoveCardToHopper,
                CardMoveCommand.MoveToContact => PrinterCommand.MoveCardToContact,
                CardMoveCommand.MoveToContactless => PrinterCommand.MoveCardToContactless,
                CardMoveCommand.MoveToStandbyBack => PrinterCommand.MoveCardToStandbyBack,
                CardMoveCommand.MoveToFlipper => PrinterCommand.MoveCardToFlipper,
                CardMoveCommand.MoveToRejectBoxFront => PrinterCommand.MoveCardToRejectBoxFront,
                CardMoveCommand.MoveToRejectBoxDown => PrinterCommand.MoveCardToRejectBoxDown,
                CardMoveCommand.MoveToStandbyFront => PrinterCommand.MoveCardToStandbyFront,
                CardMoveCommand.MoveToFront => PrinterCommand.MoveCardToFront,
                CardMoveCommand.MoveToPrepare => PrinterCommand.MoveCardToPrepare,
                CardMoveCommand.MoveToIndentPrinter => PrinterCommand.MoveCardToIndentPrinter,
                CardMoveCommand.MoveToStandbyDown => PrinterCommand.MoveCardToStandbyDown,
                CardMoveCommand.MoveToStorage => PrinterCommand.MoveCardToStorage,
                CardMoveCommand.MoveFromStorageToPrepare => PrinterCommand.MoveCardFromStorageToPrepare,
                _ => PrinterCommand.MoveCardToPrepare
            };
        }

        private static CardPositionState ConvertCardPosition(string rawPosition)
        {
            if (!int.TryParse(rawPosition, out var positionValue))
                return CardPositionState.Unknown;

            return positionValue switch
            {
                Native.CardPosition.OutOfPrinter => CardPositionState.OutOfPrinter,
                Native.CardPosition.FrontStandby => CardPositionState.FrontStandby,
                Native.CardPosition.Flipper => CardPositionState.Flipper,
                Native.CardPosition.MagIn => CardPositionState.MagneticStripeRead,
                Native.CardPosition.MagOut => CardPositionState.MagneticStripeWrite,
                Native.CardPosition.StartPrinting => CardPositionState.PrintStation,
                Native.CardPosition.PrintEnd => CardPositionState.PrintCompleted,
                Native.CardPosition.Contact => CardPositionState.Contact,
                Native.CardPosition.Contactless => CardPositionState.Contactless,
                Native.CardPosition.BackStandby => CardPositionState.BackStandby,
                Native.CardPosition.CardJamPos => CardPositionState.CardJam,
                Native.CardPosition.PreparePos => CardPositionState.Prepare,
                Native.CardPosition.StartPrinting2 => CardPositionState.PrintStation,
                Native.CardPosition.DownStandby => CardPositionState.DownStandby,
                Native.CardPosition.WaitEmboss => CardPositionState.IndentPrinter,
                _ => CardPositionState.Unknown
            };
        }

        #endregion

        #region Magnetic Stripe

        public (string Track1, string Track2, string Track3) ReadMagneticStripe(string printerId, uint mode = 0)
        {
            var track1 = new byte[MaxTrackBufferSize];
            var track2 = new byte[MaxTrackBufferSize];
            var track3 = new byte[MaxTrackBufferSize];

            var result = SeaorySdk.SOY_PR_ReadTrackA(printerId, mode, track1, track2, track3);

            if (result != 0)
                throw new SeaoryPrinterException(result, GetErrorDescription(result));

            return (
                Encoding.ASCII.GetString(track1).TrimEnd('\0'),
                Encoding.ASCII.GetString(track2).TrimEnd('\0'),
                Encoding.ASCII.GetString(track3).TrimEnd('\0')
            );
        }

        public uint WriteMagneticStripe(string printerId, uint mode, string track1, string track2, string track3)
        {
            return SeaorySdk.SOY_PR_WriteTrackA(printerId, mode, track1 ?? "", track2 ?? "", track3 ?? "");
        }

        public uint EncodeMagneticStripe(string printerId, uint mode, string track1, string track2, string track3, uint retry = 1)
        {
            return SeaorySdk.SOY_PR_EncodeTrackA(printerId, mode, track1 ?? "", track2 ?? "", track3 ?? "", retry);
        }

        #endregion

        #region Error Handling

        public static string GetErrorDescription(uint errorCode)
        {
            return errorCode switch
            {
                0 => "Success",
                ErrorCode.IncorrectFunction => "Incorrect function",
                ErrorCode.FileNotFound => "File not found",
                ErrorCode.PathNotFound => "Path not found",
                ErrorCode.AccessDenied => "Access denied - try running with elevated privileges or add user to 'lp' group",
                ErrorCode.InvalidHandle => "Invalid handle",
                ErrorCode.NotEnoughMemory => "Not enough memory",
                ErrorCode.InvalidData => "Invalid data",
                ErrorCode.OutOfMemory => "Out of memory",
                ErrorCode.DeviceNotFound => "Device not found",
                ErrorCode.DeviceNotReady => "Device not ready",
                ErrorCode.NotSupported => "Operation not supported",
                ErrorCode.InvalidParameter => "Invalid parameter",
                ErrorCode.DeviceBusy => "Device is busy - wait and retry",
                ErrorCode.WaitTimeout => "Operation timed out",
                ErrorCode.DeviceNotConnected => "Device is not connected",
                ErrorCode.ConnectionRefused => "Connection refused - check network settings",
                ErrorCode.FirmwareError => "Firmware error",
                ErrorCode.TphThermistorError => "TPH thermistor error",
                ErrorCode.EncoderErrorTake => "Encoder error (take)",
                ErrorCode.EncoderErrorSupply => "Encoder error (supply)",
                ErrorCode.CoverOpen => "Cover is open",
                ErrorCode.RejectboxOpen => "Reject box is open",
                ErrorCode.RejectboxFull => "Reject box is full",
                ErrorCode.RibbonOut => "Ribbon is out",
                ErrorCode.RibbonMissing => "Ribbon is missing",
                ErrorCode.RibbonMismatch => "Ribbon type mismatch",
                ErrorCode.CardFeedError => "Card feed error",
                ErrorCode.CardOut => "No card in printer",
                ErrorCode.FilmOut => "Film is out",
                ErrorCode.FilmMissing => "Film is missing",
                ErrorCode.PrinterLocked => "Printer is locked",
                ErrorCode.TrackDataEmpty => "Track data is empty",
                ErrorCode.MagEncodingModuleNotAttached => "Magnetic encoding module not attached",
                ErrorCode.EncodingMagStripeFail => "Failed to encode magnetic stripe",
                _ => $"Unknown error (0x{errorCode:X8})"
            };
        }

        #endregion

        #region IDisposable

        /// <inheritdoc />
        public override void Dispose()
        {
            foreach (var context in _printContexts.Values)
            {
                if (context != IntPtr.Zero)
                {
                    try
                    {
                        SeaorySdk.SOY_PR_EndPrinting2(context, true);
                    }
                    catch { }
                }
            }

            _printContexts.Clear();
            _connectedPrinters.Clear();
        }

        #endregion
    }

    public class PrintSession : CardPrintSessionBase
    {
        private readonly SeaoryPrinterDriver _driver;
        private readonly string _printerId;
        private readonly IntPtr _context;
        private bool _disposed;
        private bool _pageStarted;

        internal PrintSession(SeaoryPrinterDriver driver, string printerId, IntPtr context)
        {
            _driver = driver;
            _printerId = printerId;
            _context = context;
        }

        /// <inheritdoc />
        public override CardPrintSessionBase BeginPage()
        {
            if (_disposed)
                throw new ObjectDisposedException(nameof(PrintSession));

            if (_pageStarted)
                throw new InvalidOperationException("Page already started - call EndPage first");

            var result = _driver.StartPage(_context);
            if (result != 0)
                throw new SeaoryPrinterException(result, SeaoryPrinterDriver.GetErrorDescription(result));

            _pageStarted = true;
            return this;
        }

        /// <inheritdoc />
        public override CardPrintSessionBase EndPage()
        {
            if (_disposed)
                throw new ObjectDisposedException(nameof(PrintSession));

            if (!_pageStarted)
                throw new InvalidOperationException("No page started");

            var result = _driver.EndPage(_context);
            if (result != 0)
                throw new SeaoryPrinterException(result, SeaoryPrinterDriver.GetErrorDescription(result));

            _pageStarted = false;
            return this;
        }

        /// <inheritdoc />
        public override CardPrintSessionBase PrintImage(int x, int y, int width, int height, string imagePath)
        {
            if (_disposed)
                throw new ObjectDisposedException(nameof(PrintSession));

            if (!_pageStarted)
                throw new InvalidOperationException("Must call BeginPage first");

            var result = _driver.PrintImage(_context, x, y, width, height, imagePath);
            if (result != 0)
                throw new SeaoryPrinterException(result, SeaoryPrinterDriver.GetErrorDescription(result));

            return this;
        }

        /// <inheritdoc />
        public override CardPrintSessionBase PrintText(int x, int y, string text, string fontName = "SimSun",
            int fontSize = 10, int fontWeight = 400, CardFontAttribute attributes = CardFontAttribute.Normal,
            uint textColor = 0, bool transparentBack = true)
        {
            if (_disposed)
                throw new ObjectDisposedException(nameof(PrintSession));

            if (!_pageStarted)
                throw new InvalidOperationException("Must call BeginPage first");

            var result = _driver.PrintText(_context, x, y, text, fontName, fontSize, fontWeight, ConvertFontAttribute(attributes), textColor, transparentBack);
            if (result != 0)
                throw new SeaoryPrinterException(result, SeaoryPrinterDriver.GetErrorDescription(result));

            return this;
        }

        /// <inheritdoc />
        public override void Dispose()
        {
            if (_disposed)
                return;

            if (_pageStarted)
            {
                try { _driver.EndPage(_context); } catch { }
            }

            _driver.EndPrintSession(_context, false);
            _disposed = true;
        }

        /// <inheritdoc />
        public override void Cancel()
        {
            if (_disposed)
                return;

            _driver.EndPrintSession(_context, true);
            _disposed = true;
        }

        private static FontAttribute ConvertFontAttribute(CardFontAttribute attributes)
        {
            return (FontAttribute)(byte)attributes;
        }
    }

    public class SeaoryPrinterException : Exception
    {
        public uint ErrorCode { get; }

        public SeaoryPrinterException(uint errorCode, string message)
            : base($"Seaory Printer Error 0x{errorCode:X8}: {message}")
        {
            ErrorCode = errorCode;
        }
    }

    public class PrinterStatusEventArgs : EventArgs
    {
        public string PrinterId { get; set; } = string.Empty;
        public PrinterStatus Status { get; set; }
        public uint StatusCode { get; set; }
    }

    public class PrintJobEventArgs : EventArgs
    {
        public string JobId { get; set; } = string.Empty;
        public JobStatus Status { get; set; }
        public string? ErrorMessage { get; set; }
    }
}
