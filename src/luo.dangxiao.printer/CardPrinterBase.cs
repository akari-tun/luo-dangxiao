using System.Collections.Generic;
using System.Threading.Tasks;

namespace luo.dangxiao.printer
{
    /// <summary>
    /// Base abstraction for card printer providers.
    /// </summary>
    public abstract class CardPrinterBase : IDisposable
    {
        /// <summary>
        /// Gets the provider name.
        /// </summary>
        public abstract string ProviderName { get; }

        /// <summary>
        /// Discovers available printers for the provider.
        /// </summary>
        /// <returns>The discovered printer list.</returns>
        public abstract Task<IReadOnlyList<PrinterInfo>> DiscoverPrintersAsync();

        /// <summary>
        /// Connects to a printer.
        /// </summary>
        /// <param name="printerId">The printer identifier.</param>
        /// <returns><see langword="true"/> when the connection succeeds.</returns>
        public abstract Task<bool> ConnectAsync(string printerId);

        /// <summary>
        /// Disconnects from a printer.
        /// </summary>
        /// <param name="printerId">The printer identifier.</param>
        /// <returns><see langword="true"/> when the printer is disconnected.</returns>
        public abstract Task<bool> DisconnectAsync(string printerId);

        /// <summary>
        /// Sends a print job to a printer.
        /// </summary>
        /// <param name="printerId">The printer identifier.</param>
        /// <param name="job">The print job.</param>
        /// <returns><see langword="true"/> when the print job succeeds.</returns>
        public abstract Task<bool> SendPrintJobAsync(string printerId, PrintJob job);

        /// <summary>
        /// Gets the current printer status.
        /// </summary>
        /// <param name="printerId">The printer identifier.</param>
        /// <returns>The current status.</returns>
        public abstract Task<PrinterStatus> GetPrinterStatusAsync(string printerId);

        /// <summary>
        /// Gets printer information as raw bytes.
        /// </summary>
        /// <param name="printerId">The printer identifier.</param>
        /// <returns>The printer information payload.</returns>
        public abstract Task<byte[]> GetPrinterInfoAsync(string printerId);

        /// <summary>
        /// Starts a provider-specific print session.
        /// </summary>
        /// <param name="printerId">The printer identifier.</param>
        /// <returns>A new print session.</returns>
        public abstract CardPrintSessionBase BeginPrintSession(string printerId);

        /// <summary>
        /// Moves a card to a target printer position.
        /// </summary>
        /// <param name="printerId">The printer identifier.</param>
        /// <param name="command">The movement command.</param>
        /// <returns><see langword="true"/> when the command succeeds.</returns>
        public abstract Task<bool> MoveCardAsync(string printerId, CardMoveCommand command);

        /// <summary>
        /// Ejects the current card from the printer.
        /// </summary>
        /// <param name="printerId">The printer identifier.</param>
        /// <returns><see langword="true"/> when the eject succeeds.</returns>
        public virtual Task<bool> EjectCardAsync(string printerId)
        {
            return MoveCardAsync(printerId, CardMoveCommand.MoveToHopper);
        }

        /// <summary>
        /// Resets the printer.
        /// </summary>
        /// <param name="printerId">The printer identifier.</param>
        /// <param name="hardReset">Whether to perform a hard reset.</param>
        /// <returns><see langword="true"/> when the reset succeeds.</returns>
        public abstract Task<bool> ResetPrinterAsync(string printerId, bool hardReset = false);

        /// <summary>
        /// Gets the current card position.
        /// </summary>
        /// <param name="printerId">The printer identifier.</param>
        /// <returns>The current card position.</returns>
        public abstract Task<CardPositionState> GetCardPositionAsync(string printerId);

        /// <summary>
        /// Releases provider resources.
        /// </summary>
        public virtual void Dispose()
        {
        }
    }
}
