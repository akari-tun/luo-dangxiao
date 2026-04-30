using System.Runtime.InteropServices;
using System.Runtime.Versioning;

namespace luo.dangxiao.cardreader.Yc.Platform.Linux;

[SupportedOSPlatform("linux")]
internal sealed class LinuxHidDevice : IHidDevice
{
    private readonly ushort _vendorId;
    private readonly ushort _productId;
    private readonly string _devicePath;
    private int _fd = -1;
    private bool _disposed;

    public bool IsOpen => _fd >= 0;
    public ushort VendorId => _vendorId;
    public ushort ProductId => _productId;

    public LinuxHidDevice(ushort vendorId, ushort productId)
    {
        _vendorId = vendorId;
        _productId = productId;
        _devicePath = FindDevicePath(vendorId, productId) 
            ?? throw new InvalidOperationException($"HID device not found (VID:{vendorId:X4}, PID:{productId:X4})");
    }

    public LinuxHidDevice(string devicePath)
    {
        _devicePath = devicePath ?? throw new ArgumentNullException(nameof(devicePath));
        _vendorId = 0;
        _productId = 0;
    }

    public bool Open()
    {
        if (_disposed) throw new ObjectDisposedException(nameof(LinuxHidDevice));
        if (IsOpen) return true;

        _fd = NativeMethods.open(_devicePath, NativeMethods.O_RDWR);
        return _fd >= 0;
    }

    public void Close()
    {
        if (_fd >= 0)
        {
            NativeMethods.close(_fd);
            _fd = -1;
        }
    }

    public int Read(byte[] buffer, int timeoutMs = 500)
    {
        if (_disposed) throw new ObjectDisposedException(nameof(LinuxHidDevice));
        if (!IsOpen) throw new InvalidOperationException("Device not open");

        // Linux hidraw returns 64 bytes directly without report ID prefix
        byte[] readBuffer = new byte[64];
        
        nint bytesRead = NativeMethods.read(_fd, readBuffer, 64);
        
        if (bytesRead < 0)
        {
            ReaderLogger.Log($"[LinuxHidDevice.Read] ERROR: read returned {bytesRead}");
            return -1;
        }

        ReaderLogger.Log($"[LinuxHidDevice.Read] Raw {bytesRead} bytes from HID");
        ReaderLogger.Log($"[LinuxHidDevice.Read] Full buffer: {BitConverter.ToString(readBuffer, 0, Math.Min((int)bytesRead, 64))}");
        
        // Copy all bytes (Linux hidraw doesn't have report ID like Windows)
        if (bytesRead > 0)
        {
            int copyLength = Math.Min((int)bytesRead, buffer.Length);
            Buffer.BlockCopy(readBuffer, 0, buffer, 0, copyLength);
            ReaderLogger.Log($"[LinuxHidDevice.Read] Copied {copyLength} bytes to output buffer");
            return copyLength;
        }
        
        return 0;
    }

    public int Write(byte[] buffer, int timeoutMs = 500)
    {
        if (_disposed) throw new ObjectDisposedException(nameof(LinuxHidDevice));
        if (!IsOpen) throw new InvalidOperationException("Device not open");

        // Linux hidraw expects same format as Windows: [report_id][length][data...]
        // Total buffer must be 65 bytes for the device
        byte[] writeBuffer = new byte[65];
        writeBuffer[0] = 0x00;  // Report ID
        writeBuffer[1] = (byte)buffer.Length;  // Length byte
        int copyLen = Math.Min(buffer.Length, 63);  // Max 63 bytes after report_id and length
        Buffer.BlockCopy(buffer, 0, writeBuffer, 2, copyLen);

        ReaderLogger.Log($"[LinuxHidDevice.Write] Writing 65 bytes (report_id + length + {copyLen} data): {BitConverter.ToString(writeBuffer, 0, Math.Min(30, 65))}");

        nint bytesWritten = NativeMethods.write(_fd, writeBuffer, 65);
        
        if (bytesWritten < 0)
        {
            ReaderLogger.Log($"[LinuxHidDevice.Write] ERROR: write returned {bytesWritten}");
            return -1;
        }

        ReaderLogger.Log($"[LinuxHidDevice.Write] Success: {bytesWritten} bytes written");
        return copyLen;
    }

    private static string? FindDevicePath(ushort vendorId, ushort productId)
    {
        // Check up to 256 hidraw devices
        for (int i = 0; i < 256; i++)
        {
            string path = $"/dev/hidraw{i}";
            
            if (!File.Exists(path))
                continue;

            int fd = -1;
            try
            {
                fd = NativeMethods.open(path, NativeMethods.O_RDONLY);
                if (fd < 0)
                    continue;

                var devInfo = new NativeMethods.HidrawDevinfo();
                if (NativeMethods.ioctl(fd, NativeMethods.HidioCgrawinfo, ref devInfo) >= 0)
                {
                    // Vendor and Product are signed shorts in kernel struct
                    // Cast to ushort to get correct values
                    ushort vid = (ushort)devInfo.Vendor;
                    ushort pid = (ushort)devInfo.Product;

                    System.Diagnostics.Debug.WriteLine($"[LinuxHidDevice] Checking {path}: VID={vid:X4}, PID={pid:X4}");

                    if (vid == vendorId && pid == productId)
                    {
                        System.Diagnostics.Debug.WriteLine($"[LinuxHidDevice] Found device at {path}");
                        return path;
                    }
                }
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine($"[LinuxHidDevice] Error accessing {path}: {ex.Message}");
            }
            finally
            {
                if (fd >= 0)
                    NativeMethods.close(fd);
            }
        }

        System.Diagnostics.Debug.WriteLine($"[LinuxHidDevice] Device not found (VID:{vendorId:X4}, PID:{productId:X4})");
        return null;
    }

    public void Dispose()
    {
        if (_disposed) return;
        Close();
        _disposed = true;
    }
}
