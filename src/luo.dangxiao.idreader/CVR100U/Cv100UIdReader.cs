using luo.dangxiao.idreader.CVR100U.Native;
using System.Runtime.InteropServices;
using System.Text;

namespace luo.dangxiao.idreader.CVR100U;

/// <summary>
/// CVR100U hardware ID reader implementation.
/// Supports Linux (lib100UD.so) and Windows (Termb.dll, with x86/x64 resolution).
/// </summary>
public sealed class Cv100UIdReader : IdReaderBase
{
    private static readonly ICvr100UMethods _sdk;

    static Cv100UIdReader()
    {
        if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
        {
            _sdk = Environment.Is64BitProcess
                ? new WindowsX64Cvr100UMethods()
                : new WindowsX86Cvr100UMethods();
        }
        else if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
        {
            _sdk = new LinuxCvr100UMethods();
        }
        else
        {
            _sdk = null!; // Throw on usage
        }
    }

    private const int SuccessCode = 1;
    private const int LinuxUsbOtgProtocolType = 2;
    private const int WindowsUsbPort = 1001;
    private const int DefaultTextBufferSize = 64;
    private const int PhotoBufferSize = 40000;
    private const int DefaultBinaryBufferSize = 16;

    private readonly object _lock = new();
    private bool _isInitialized;
    private bool _disposed;
    private string? _samId;

    /// <inheritdoc />
    public override string ProviderName => "HuashiCvr100U";

    /// <inheritdoc />
    public override bool Init()
    {
        lock (_lock)
        {
            if (_disposed)
            {
                return false;
            }

            if (_isInitialized)
            {
                return true;
            }

            if (_sdk is null)
            {
                return false;
            }

            try
            {
                int result = RuntimeInformation.IsOSPlatform(OSPlatform.Linux)
                    ? _sdk.CVR_InitComm(null, LinuxUsbOtgProtocolType)
                    : _sdk.CVR_InitComm(WindowsUsbPort);

                if (result != SuccessCode)
                {
                    return false;
                }

                _samId = ReadSdkString(_sdk.CVR_GetSAMID);
                _isInitialized = true;
                return true;
            }
            catch (DllNotFoundException)
            {
                return false;
            }
            catch (EntryPointNotFoundException)
            {
                return false;
            }
            catch (BadImageFormatException)
            {
                return false;
            }
            catch (ExternalException)
            {
                return false;
            }
        }
    }

    /// <inheritdoc />
    public override bool ReadIdCard(out IdCardData data)
    {
        lock (_lock)
        {
            data = default;

            if (_disposed || !_isInitialized)
            {
                return false;
            }

            try
            {
                if (_sdk.CVR_Authenticate() != SuccessCode)
                {
                    return false;
                }

                if (_sdk.CVR_Read_Content(1) != SuccessCode)
                {
                    return false;
                }

                byte[] certTypeBytes = ReadSdkByteArray(_sdk.GetCertType, DefaultBinaryBufferSize);
                byte[] photoBytes = ReadSdkByteArray(_sdk.GetBMPData, PhotoBufferSize);
                byte[] uidBytes = ReadSdkByteArray(_sdk.CVR_GetUID, 8);
                string uid = uidBytes.Length > 0 ? Convert.ToHexString(uidBytes) : string.Empty;

                data = new IdCardData
                {
                    Name = ReadSdkString(_sdk.GetPeopleName),
                    Gender = ReadSdkString(_sdk.GetPeopleSex),
                    Nation = ReadSdkString(_sdk.GetPeopleNation),
                    Birthday = ReadSdkString(_sdk.GetPeopleBirthday),
                    IdNumber = ReadSdkString(_sdk.GetPeopleIDCode),
                    Address = ReadSdkString(_sdk.GetPeopleAddress),
                    Department = ReadSdkString(_sdk.GetDepartment),
                    StartDate = ReadSdkString(_sdk.GetStartDate),
                    EndDate = ReadSdkString(_sdk.GetEndDate),
                    PhotoBytes = photoBytes.Length == 0 ? null : photoBytes,
                    CertType = ParseCertType(certTypeBytes),
                    Uid = string.IsNullOrWhiteSpace(uid) ? null : uid,
                    SamId = string.IsNullOrWhiteSpace(_samId) ? null : _samId,
                };

                return true;
            }
            catch (DllNotFoundException)
            {
                return false;
            }
            catch (EntryPointNotFoundException)
            {
                return false;
            }
            catch (BadImageFormatException)
            {
                return false;
            }
            catch (ExternalException)
            {
                return false;
            }
        }
    }

    /// <inheritdoc />
    public override void Close()
    {
        lock (_lock)
        {
            if (!_isInitialized)
            {
                return;
            }

            try
            {
                _sdk.CVR_CloseComm();
            }
            catch (DllNotFoundException)
            {
            }
            catch (EntryPointNotFoundException)
            {
            }
            catch (BadImageFormatException)
            {
            }
            catch (ExternalException)
            {
            }
            finally
            {
                _isInitialized = false;
                _samId = null;
            }
        }
    }

    /// <inheritdoc />
    public override void Dispose()
    {
        lock (_lock)
        {
            if (_disposed)
            {
                return;
            }

            try
            {
                if (_isInitialized)
                {
                    Close();
                }
            }
            finally
            {
                _disposed = true;
            }
        }
    }

    private static IdTypeEnum ParseCertType(byte[] certTypeBytes)
    {
        if (certTypeBytes.Length == 0)
        {
            return IdTypeEnum.ChineseId;
        }

        string text = Encoding.ASCII.GetString(certTypeBytes).TrimEnd('\0').Trim();
        if (int.TryParse(text, out int parsedType))
        {
            return MapCertType(parsedType);
        }

        return MapCertType(certTypeBytes[0]);
    }

    private static IdTypeEnum MapCertType(int certType)
    {
        return certType switch
        {
            1 => IdTypeEnum.ForeignPermanentResident,
            2 => IdTypeEnum.HkMacaoTaiwanResident,
            4 => IdTypeEnum.NewForeignPermanentResident,
            _ => IdTypeEnum.ChineseId,
        };
    }

    private static string ReadSdkString(SdkStringGetter getter, int bufferSize = DefaultTextBufferSize)
    {
        var buffer = new StringBuilder(bufferSize);
        int length = bufferSize;
        int result = getter(buffer, ref length);
        if (result == SuccessCode && buffer.Length > 0)
        {
            return buffer.ToString().TrimEnd('\0');
        }

        return string.Empty;
    }

    private static byte[] ReadSdkByteArray(SdkByteGetter getter, int bufferSize)
    {
        var buffer = new byte[bufferSize];
        int length = bufferSize;
        int result = getter(buffer, ref length);
        if (result == SuccessCode && length > 0)
        {
            int actualLength = Math.Min(length, buffer.Length);
            return actualLength == buffer.Length ? buffer : buffer[..actualLength];
        }

        return Array.Empty<byte>();
    }

    private delegate int SdkStringGetter(StringBuilder buffer, ref int length);

    private delegate int SdkByteGetter(byte[] buffer, ref int length);
}
