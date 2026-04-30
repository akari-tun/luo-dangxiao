// ================================================================================
// YCCARD Error Codes
// Smart Card Reader Error Definitions
// ================================================================================

namespace luo.dangxiao.cardreader.Yc.Protocol;

/// <summary>
/// Error codes returned by card reader operations
/// </summary>
public enum ErrorCode : int
{
    /// <summary>
    /// Operation successful
    /// </summary>
    Success = 0,

    /// <summary>
    /// MI OK (compatible with original library)
    /// </summary>
    MiOk = 0,

    /// <summary>
    /// DAS OK (compatible with original library)
    /// </summary>
    DasOk = 0,

    /// <summary>
    /// Communication error
    /// </summary>
    CommunicationError = -1,

    /// <summary>
    /// Reader device error
    /// </summary>
    ReaderError = -2,

    /// <summary>
    /// No payment system authorization
    /// </summary>
    NoPaymentAuthorization = -3,

    /// <summary>
    /// No access control authorization
    /// </summary>
    NoAccessControlAuthorization = -4,

    /// <summary>
    /// Parameter error
    /// </summary>
    ParameterError = -5,

    /// <summary>
    /// Timeout error
    /// </summary>
    TimeoutError = -6,

    /// <summary>
    /// No card present
    /// </summary>
    NoCard = -7,

    /// <summary>
    /// No access control system card
    /// </summary>
    NoAccessControlSystemCard = -8,

    /// <summary>
    /// No payment system card
    /// </summary>
    NoPaymentSystemCard = -9,

    /// <summary>
    /// System card error
    /// </summary>
    SystemCardError = -10,

    /// <summary>
    /// User card error
    /// </summary>
    UserCardError = -11,

    /// <summary>
    /// Card read error
    /// </summary>
    ReadCardError = -12,

    /// <summary>
    /// Card write error
    /// </summary>
    WriteCardError = -13,

    /// <summary>
    /// License creation error
    /// </summary>
    CreateLicenseError = -14,

    /// <summary>
    /// Not identified
    /// </summary>
    NotIdentified = -15,

    /// <summary>
    /// No license error
    /// </summary>
    NoLicenseError = -16,

    /// <summary>
    /// Key file not found
    /// </summary>
    NoKeyError = -17,

    /// <summary>
    /// No water billing system card
    /// </summary>
    NoWaterBillingSystemCard = -18,

    /// <summary>
    /// No water billing authorization
    /// </summary>
    NoWaterBillingAuthorization = -19,

    /// <summary>
    /// Card type error
    /// </summary>
    CardTypeError = -20,

    /// <summary>
    /// No CPU card
    /// </summary>
    NoCpuCard = -21,

    /// <summary>
    /// Select file error
    /// </summary>
    SelectFileError = -22,

    /// <summary>
    /// Get challenge error
    /// </summary>
    GetChallengeError = -23,

    /// <summary>
    /// External authentication error
    /// </summary>
    ExternalAuthenticationError = -24,

    /// <summary>
    /// Update error
    /// </summary>
    UpdateError = -25,

    /// <summary>
    /// Read binary error
    /// </summary>
    ReadBinaryError = -26,

    /// <summary>
    /// User card type
    /// </summary>
    UserCard = -27,

    /// <summary>
    /// Blank card
    /// </summary>
    BlankCard = -28,

    /// <summary>
    /// Make user card error
    /// </summary>
    MakeUserCardError = -29,

    /// <summary>
    /// Password error
    /// </summary>
    PasswordError = -30,

    /// <summary>
    /// Receive data error
    /// </summary>
    ReceiveDataError = -38,

    /// <summary>
    /// Write error
    /// </summary>
    WriteError = -61,

    /// <summary>
    /// Command start error
    /// </summary>
    CommandStartError = -70,

    /// <summary>
    /// Device address error
    /// </summary>
    DeviceAddressError = -71,

    /// <summary>
    /// Control word error
    /// </summary>
    ControlWordError = -72,

    /// <summary>
    /// Control failed
    /// </summary>
    ControlFailed = -73,

    /// <summary>
    /// RT communication error
    /// </summary>
    RtCommunicationError = -74,

    /// <summary>
    /// CRC error
    /// </summary>
    CrcError = -75,

    /// <summary>
    /// Command end error
    /// </summary>
    CommandEndError = -76,

    /// <summary>
    /// Receive length error
    /// </summary>
    ReceiveLengthError = -77,

    /// <summary>
    /// USB data error 1
    /// </summary>
    UsbDataError1 = -81,

    /// <summary>
    /// USB data error 2
    /// </summary>
    UsbDataError2 = -82,

    /// <summary>
    /// USB data error 3
    /// </summary>
    UsbDataError3 = -83,

    /// <summary>
    /// Read USB file error
    /// </summary>
    ReadUsbFileError = -84,

    /// <summary>
    /// Write USB file error
    /// </summary>
    WriteUsbFileError = -85
}
