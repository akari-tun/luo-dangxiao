// ================================================================================
// YCCARD Protocol Constants
// Smart Card Reader HID Protocol Definitions
// ================================================================================

namespace luo.dangxiao.cardreader.Yc.Protocol;

/// <summary>
/// USB device identifiers for the smart card reader
/// </summary>
public static class UsbIdentifiers
{
    /// <summary>
    /// USB Vendor ID
    /// </summary>
    public const ushort VendorId = 0x2011;

    /// <summary>
    /// USB Product ID
    /// </summary>
    public const ushort ProductId = 0x0318;

    /// <summary>
    /// Maximum USB buffer size
    /// </summary>
    public const int MaxUsbBufferSize = 64;
}

/// <summary>
/// Protocol command constants
/// </summary>
public static class ProtocolCommands
{
    /// <summary>
    /// Command start marker
    /// </summary>
    public const ushort CommandStart = 0xAA5A;

    /// <summary>
    /// Command end marker
    /// </summary>
    public const ushort CommandEnd = 0xBB6B;

    /// <summary>
    /// Default device address
    /// </summary>
    public const ushort DeviceAddress = 0x0000;
}

/// <summary>
/// Control word definitions for HID commands
/// </summary>
public static class ControlWords
{
    public const string OpenRf = "OPEN_RF";
    public const byte OpenRfCode = 0x20;

    public const string Request = "RF_REQUEST";
    public const byte RequestCode = 0x21;

    public const string Anticoll = "RF_ANTICOLL";
    public const byte AnticollCode = 0x22;

    public const string Select = "RF_SELECT";
    public const byte SelectCode = 0x23;

    public const string Authentication = "RF_AUTHENTICATION";
    public const byte AuthenticationCode = 0x24;

    public const string Read = "RF_READ";
    public const byte ReadCode = 0x25;

    public const string Write = "RF_WRITE";
    public const byte WriteCode = 0x26;

    public const string Halt = "RF_HALT";
    public const byte HaltCode = 0x27;

    public const string LoadKey = "RF_LOAD_KEY";
    public const byte LoadKeyCode = 0x28;

    public const string SetTime = "SET_TIME";
    public const byte SetTimeCode = 0x13;

    public const string GetTime = "GET_TIME";
    public const byte GetTimeCode = 0x14;

    public const string SetBell = "SET_BELL";
    public const byte SetBellCode = 0x15;

    public const string SetDisplay = "SET_DISPLAY";
    public const byte SetDisplayCode = 0x16;
}

/// <summary>
/// Authentication key types
/// </summary>
public static class KeyTypes
{
    public const byte KeyA = 0x00;
    public const byte KeyB = 0x04;
    public const byte KeySet0 = 0x00;
    public const byte KeySet1 = 0x01;
    public const byte KeySet2 = 0x02;
}

/// <summary>
/// Card request modes
/// </summary>
public static class RequestModes
{
    /// <summary>
    /// Request all cards
    /// </summary>
    public const byte All = 0x52;

    /// <summary>
    /// Request idle cards
    /// </summary>
    public const byte Idle = 0x26;
}

/// <summary>
/// System type identifiers
/// </summary>
public static class SystemTypes
{
    public const int AccessControl = 1;    // MJXT - 门禁系统
    public const int Payment = 2;          // SFXT - 收费系统
    public const int WaterBilling = 3;     // JSXT - 计水系统
    public const int AccessControlAlt = 4; // MJDT - 门禁系统(备用)
}

/// <summary>
/// Sector definitions for different systems
/// </summary>
public static class SystemSectors
{
    public const int AccessControl = 2;    // MJ_Sec
    public const int Payment = 1;          // SF_Sec
    public const int WaterBilling = 3;     // JS_Sec
    public const int AccessControlAlt = 4; // DT_Sec
}
