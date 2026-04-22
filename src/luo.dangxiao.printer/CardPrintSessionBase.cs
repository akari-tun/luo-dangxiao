namespace luo.dangxiao.printer
{
    /// <summary>
    /// Base abstraction for vendor-specific card print sessions.
    /// </summary>
    public abstract class CardPrintSessionBase : IDisposable
    {
        /// <summary>
        /// Starts a print page in the current session.
        /// </summary>
        /// <returns>The current print session.</returns>
        public abstract CardPrintSessionBase BeginPage();

        /// <summary>
        /// Ends the current print page.
        /// </summary>
        /// <returns>The current print session.</returns>
        public abstract CardPrintSessionBase EndPage();

        /// <summary>
        /// Prints an image to the current page.
        /// </summary>
        /// <param name="x">The X position.</param>
        /// <param name="y">The Y position.</param>
        /// <param name="width">The width.</param>
        /// <param name="height">The height.</param>
        /// <param name="imagePath">The image path.</param>
        /// <returns>The current print session.</returns>
        public abstract CardPrintSessionBase PrintImage(int x, int y, int width, int height, string imagePath);

        /// <summary>
        /// Prints text to the current page.
        /// </summary>
        /// <param name="x">The X position.</param>
        /// <param name="y">The Y position.</param>
        /// <param name="text">The text to print.</param>
        /// <param name="fontName">The font name.</param>
        /// <param name="fontSize">The font size.</param>
        /// <param name="fontWeight">The font weight.</param>
        /// <param name="attributes">The font attributes.</param>
        /// <param name="textColor">The text color.</param>
        /// <param name="transparentBack">Whether the background is transparent.</param>
        /// <returns>The current print session.</returns>
        public abstract CardPrintSessionBase PrintText(
            int x,
            int y,
            string text,
            string fontName = "SimSun",
            int fontSize = 10,
            int fontWeight = 400,
            CardFontAttribute attributes = CardFontAttribute.Normal,
            uint textColor = 0,
            bool transparentBack = true);

        /// <summary>
        /// Cancels the current print session.
        /// </summary>
        public abstract void Cancel();

        /// <summary>
        /// Releases resources associated with the session.
        /// </summary>
        public abstract void Dispose();
    }
}
