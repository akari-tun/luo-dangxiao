using System;

namespace luo.dangxiao.printer
{
    /// <summary>
    /// Common printer information for card printers.
    /// </summary>
    public sealed class PrinterInfo
    {
        /// <summary>
        /// Gets or sets the printer provider name.
        /// </summary>
        public string ProviderName { get; set; } = string.Empty;

        /// <summary>
        /// Gets or sets the unique printer identifier.
        /// </summary>
        public string Id { get; set; } = string.Empty;

        /// <summary>
        /// Gets or sets the display name of the printer.
        /// </summary>
        public string Name { get; set; } = string.Empty;

        /// <summary>
        /// Gets or sets the printer model.
        /// </summary>
        public string Model { get; set; } = string.Empty;

        /// <summary>
        /// Gets or sets the printer address or port.
        /// </summary>
        public string Address { get; set; } = string.Empty;

        /// <summary>
        /// Gets or sets a value indicating whether the printer is connected.
        /// </summary>
        public bool IsConnected { get; set; }

        /// <summary>
        /// Gets or sets the current printer status.
        /// </summary>
        public PrinterStatus Status { get; set; }

        /// <summary>
        /// Gets or sets the firmware version.
        /// </summary>
        public string FirmwareVersion { get; set; } = string.Empty;

        /// <summary>
        /// Gets or sets the printer serial number.
        /// </summary>
        public string SerialNumber { get; set; } = string.Empty;

        /// <summary>
        /// Gets or sets the last seen time of the printer.
        /// </summary>
        public DateTime LastSeen { get; set; }
    }

    /// <summary>
    /// Common printer status values.
    /// </summary>
    public enum PrinterStatus
    {
        Offline,
        Online,
        Printing,
        Paused,
        Error,
        Ready
    }

    /// <summary>
    /// Represents a printer job for card printing.
    /// </summary>
    public sealed class PrintJob
    {
        /// <summary>
        /// Gets or sets the job identifier.
        /// </summary>
        public string Id { get; set; } = string.Empty;

        /// <summary>
        /// Gets or sets the target printer identifier.
        /// </summary>
        public string PrinterId { get; set; } = string.Empty;

        /// <summary>
        /// Gets or sets the display name of the print job.
        /// </summary>
        public string JobName { get; set; } = string.Empty;

        /// <summary>
        /// Gets or sets the raw print data.
        /// </summary>
        public byte[] PrintData { get; set; } = Array.Empty<byte>();

        /// <summary>
        /// Gets or sets the current job status.
        /// </summary>
        public JobStatus Status { get; set; }

        /// <summary>
        /// Gets or sets the creation time.
        /// </summary>
        public DateTime CreatedAt { get; set; }

        /// <summary>
        /// Gets or sets the print start time.
        /// </summary>
        public DateTime? StartedAt { get; set; }

        /// <summary>
        /// Gets or sets the print completion time.
        /// </summary>
        public DateTime? CompletedAt { get; set; }

        /// <summary>
        /// Gets or sets the error message when the print job fails.
        /// </summary>
        public string? ErrorMessage { get; set; }
    }

    /// <summary>
    /// Common print job status values.
    /// </summary>
    public enum JobStatus
    {
        Queued,
        Processing,
        Printing,
        Completed,
        Failed,
        Cancelled
    }

    /// <summary>
    /// Supported card movement commands.
    /// </summary>
    public enum CardMoveCommand
    {
        MoveToHopper,
        MoveToContact,
        MoveToContactless,
        MoveToStandbyBack,
        MoveToFlipper,
        MoveToRejectBoxFront,
        MoveToRejectBoxDown,
        MoveToStandbyFront,
        MoveToFront,
        MoveToPrepare,
        MoveToIndentPrinter,
        MoveToStandbyDown,
        MoveToStorage,
        MoveFromStorageToPrepare
    }

    /// <summary>
    /// Common card position states inside a card printer.
    /// </summary>
    public enum CardPositionState
    {
        Unknown,
        OutOfPrinter,
        FrontStandby,
        BackStandby,
        Flipper,
        MagneticStripeRead,
        MagneticStripeWrite,
        PrintStation,
        PrintCompleted,
        Contact,
        Contactless,
        CardJam,
        Prepare,
        DownStandby,
        IndentPrinter,
        Front,
        RejectBox,
        Storage
    }

    /// <summary>
    /// Font attributes for card printer text rendering.
    /// </summary>
    [Flags]
    public enum CardFontAttribute : byte
    {
        Normal = 0x00,
        Bold = 0x01,
        Underline = 0x20,
        Italic = 0x40
    }
}
