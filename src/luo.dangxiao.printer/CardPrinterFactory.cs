using luo.dangxiao.printer.Seaory;
using luo.dangxiao.printer.Virtual;

namespace luo.dangxiao.printer
{
    /// <summary>
    /// Creates card printer implementations based on configuration.
    /// </summary>
    public static class CardPrinterFactory
    {
        /// <summary>
        /// Creates a card printer implementation.
        /// </summary>
        /// <param name="provider">The configured printer provider enum value.</param>
        /// <returns>The matching card printer implementation.</returns>
        public static CardPrinterBase Create(Enum? provider = null)
        {
            return provider?.ToString() switch
            {
                "Virtual" => new VirtualCardPrinter(),
                "Seaory" or null or "" => new SeaoryPrinterDriver(),
                _ => new SeaoryPrinterDriver()
            };
        }
    }
}
