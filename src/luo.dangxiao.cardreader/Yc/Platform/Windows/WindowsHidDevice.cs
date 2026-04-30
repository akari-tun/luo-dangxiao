using System.Runtime.InteropServices;
using System.Runtime.Versioning;
using Microsoft.Win32.SafeHandles;

namespace luo.dangxiao.cardreader.Yc.Platform.Windows;

[SupportedOSPlatform("windows")]
internal sealed class WindowsHidDevice : IHidDevice
{
    private readonly ushort _vendorId;
    private readonly ushort _productId;
    private readonly string _devicePath;
    private SafeFileHandle? _handle;
    private bool _disposed;

    public bool IsOpen => _handle != null && !_handle.IsInvalid && !_handle.IsClosed;
    public ushort VendorId => _vendorId;
    public ushort ProductId => _productId;

    public WindowsHidDevice(ushort vendorId, ushort productId)
    {
        _vendorId = vendorId;
        _productId = productId;
        _devicePath = FindDevicePath(vendorId, productId) 
            ?? throw new InvalidOperationException($"HID device not found (VID:{vendorId:X4}, PID:{productId:X4})");
    }

    public WindowsHidDevice(string devicePath)
    {
        _devicePath = devicePath ?? throw new ArgumentNullException(nameof(devicePath));
        _vendorId = 0;
        _productId = 0;
    }

    public bool Open()
    {
        if (_disposed) throw new ObjectDisposedException(nameof(WindowsHidDevice));
        if (IsOpen) return true;

        _handle = NativeMethods.CreateFile(
            _devicePath,
            NativeMethods.FileAccess.GenericReadWrite,
            NativeMethods.FileShare.ReadWrite,
            IntPtr.Zero,
            NativeMethods.CreationDisposition.OpenExisting,
            NativeMethods.FileAttributes.Normal,
            IntPtr.Zero);

        return !_handle.IsInvalid;
    }

    public void Close()
    {
        if (_handle != null && !_handle.IsInvalid)
        {
            NativeMethods.CancelIo(_handle);
            _handle.Dispose();
            _handle = null;
        }
    }

    public int Read(byte[] buffer, int timeoutMs = 500)
    {
        if (_disposed) throw new ObjectDisposedException(nameof(WindowsHidDevice));
        if (!IsOpen) throw new InvalidOperationException("Device not open");

        // HID requires buffer to be (report_size + 1) bytes for report ID prefix
        // The device uses 64-byte reports, so we need 65 bytes
        byte[] readBuffer = new byte[65];

        uint bytesRead;
        bool success = NativeMethods.ReadFile(_handle!, readBuffer, 65, out bytesRead, IntPtr.Zero);

        if (!success)
        {
            int error = Marshal.GetLastWin32Error();
            if (error == 997)
            {
                return 0;
            }
            return -1;
        }

        // First byte is report ID, actual data starts at index 1
        if (bytesRead > 1)
        {
            int dataLength = (int)bytesRead - 1;
            int copyLength = Math.Min(dataLength, buffer.Length);
            Buffer.BlockCopy(readBuffer, 1, buffer, 0, copyLength);
            return copyLength;
        }

        return 0;
    }

    public int Write(byte[] buffer, int timeoutMs = 500)
    {
        if (_disposed) throw new ObjectDisposedException(nameof(WindowsHidDevice));
        if (!IsOpen) throw new InvalidOperationException("Device not open");

        // C++ CHidCmd::WriteFile format: [report_id=0][length][data...]
        // Total buffer is 65 bytes
        byte[] writeBuffer = new byte[65];
        writeBuffer[0] = 0x00;  // Report ID
        writeBuffer[1] = (byte)buffer.Length;  // Length byte (matching C++ implementation)
        int copyLen = Math.Min(buffer.Length, 63);  // Max 63 bytes of data after report_id and length
        Buffer.BlockCopy(buffer, 0, writeBuffer, 2, copyLen);

        uint bytesWritten;
        bool success = NativeMethods.WriteFile(_handle!, writeBuffer, 65, out bytesWritten, IntPtr.Zero);

        if (!success)
        {
            return -1;
        }

        return copyLen;
    }

    private static string? FindDevicePath(ushort vendorId, ushort productId)
    {
        NativeMethods.HidD_GetHidGuid(out Guid hidGuid);

        using SafeDeviceInfoSetHandle deviceInfoSet = NativeMethods.SetupDiGetClassDevs(
            ref hidGuid,
            null,
            IntPtr.Zero,
            NativeMethods.SetupApiFlags.DigcfPresent | NativeMethods.SetupApiFlags.DigcfDeviceInterface);

        if (deviceInfoSet.IsInvalid) return null;

        uint memberIndex = 0;
        while (true)
        {
            var interfaceData = new SpDeviceInterfaceData();
            if (!NativeMethods.SetupDiEnumDeviceInterfaces(deviceInfoSet, IntPtr.Zero, in hidGuid, memberIndex, ref interfaceData))
                break;

            memberIndex++;

            uint requiredSize = 0;
            if (!NativeMethods.SetupDiGetDeviceInterfaceDetail(deviceInfoSet, ref interfaceData, IntPtr.Zero, 0, out requiredSize, IntPtr.Zero))
            {
                if (Marshal.GetLastWin32Error() != 122) continue;
            }

            if (requiredSize == 0) continue;

            IntPtr detailDataBuffer = Marshal.AllocHGlobal((int)requiredSize);
            try
            {
                int cbSize = IntPtr.Size == 8 ? 8 : 4;
                Marshal.WriteInt32(detailDataBuffer, cbSize);

                if (!NativeMethods.SetupDiGetDeviceInterfaceDetail(deviceInfoSet, ref interfaceData, detailDataBuffer, requiredSize, out _, IntPtr.Zero))
                    continue;

                int pathOffset = 4;
                string? devicePath = Marshal.PtrToStringUni(detailDataBuffer + pathOffset);

                if (string.IsNullOrEmpty(devicePath)) continue;

                using SafeFileHandle testHandle = NativeMethods.CreateFile(
                    devicePath,
                    0,
                    NativeMethods.FileShare.ReadWrite,
                    IntPtr.Zero,
                    NativeMethods.CreationDisposition.OpenExisting,
                    NativeMethods.FileAttributes.Normal,
                    IntPtr.Zero);

                if (testHandle.IsInvalid) continue;

                var attrs = new HidAttributes();
                if (NativeMethods.HidD_GetAttributes(testHandle, ref attrs))
                {
                    if (attrs.VendorId == vendorId && attrs.ProductId == productId)
                    {
                        return devicePath;
                    }
                }
            }
            finally
            {
                Marshal.FreeHGlobal(detailDataBuffer);
            }
        }

        return null;
    }

    public void Dispose()
    {
        if (_disposed) return;
        Close();
        _disposed = true;
    }
}
