using System.Runtime.InteropServices;
using luo.dangxiao.cardreader.Yc.Platform.Linux;
using luo.dangxiao.cardreader.Yc.Platform.Windows;

namespace luo.dangxiao.cardreader.Yc.Platform;

public static class HidDeviceFactory
{
    public static IHidDevice Create(ushort vendorId, ushort productId)
    {
        if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
        {
            return new WindowsHidDevice(vendorId, productId);
        }
        
        if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
        {
            return new LinuxHidDevice(vendorId, productId);
        }

        throw new PlatformNotSupportedException("HID is only supported on Windows and Linux");
    }

    public static IHidDevice Create(string devicePath)
    {
        if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
        {
            return new WindowsHidDevice(devicePath);
        }
        
        if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
        {
            return new LinuxHidDevice(devicePath);
        }

        throw new PlatformNotSupportedException("HID is only supported on Windows and Linux");
    }

    public static IEnumerable<HidDeviceInfo> EnumerateDevices(ushort vendorId, ushort productId)
    {
        if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
        {
            return EnumerateWindowsDevices(vendorId, productId);
        }
        
        if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
        {
            return EnumerateLinuxDevices(vendorId, productId);
        }

        return Enumerable.Empty<HidDeviceInfo>();
    }

    [System.Runtime.Versioning.SupportedOSPlatform("windows")]
    private static IEnumerable<HidDeviceInfo> EnumerateWindowsDevices(ushort vendorId, ushort productId)
    {
        var devices = new List<HidDeviceInfo>();
        
        Windows.NativeMethods.HidD_GetHidGuid(out Guid hidGuid);

        using var deviceInfoSet = Windows.NativeMethods.SetupDiGetClassDevs(
            ref hidGuid,
            null,
            IntPtr.Zero,
            Windows.NativeMethods.SetupApiFlags.DigcfPresent | Windows.NativeMethods.SetupApiFlags.DigcfDeviceInterface);

        if (deviceInfoSet.IsInvalid) return devices;

        uint memberIndex = 0;
        while (true)
        {
            var interfaceData = new Windows.SpDeviceInterfaceData();
            if (!Windows.NativeMethods.SetupDiEnumDeviceInterfaces(deviceInfoSet, IntPtr.Zero, in hidGuid, memberIndex, ref interfaceData))
                break;

            memberIndex++;

            if (!Windows.NativeMethods.SetupDiGetDeviceInterfaceDetail(deviceInfoSet, ref interfaceData, IntPtr.Zero, 0, out uint requiredSize, IntPtr.Zero))
            {
                if (Marshal.GetLastWin32Error() != 122) continue;
            }

            IntPtr detailDataBuffer = Marshal.AllocHGlobal((int)requiredSize);
            try
            {
                Marshal.WriteInt32(detailDataBuffer, IntPtr.Size == 8 ? 8 : 6);

                if (!Windows.NativeMethods.SetupDiGetDeviceInterfaceDetail(deviceInfoSet, ref interfaceData, detailDataBuffer, requiredSize, out _, IntPtr.Zero))
                    continue;

                string devicePath = Marshal.PtrToStringAuto(detailDataBuffer + IntPtr.Size)!;

                using var testHandle = Windows.NativeMethods.CreateFile(
                    devicePath,
                    0,
                    Windows.NativeMethods.FileShare.ReadWrite,
                    IntPtr.Zero,
                    Windows.NativeMethods.CreationDisposition.OpenExisting,
                    Windows.NativeMethods.FileAttributes.Normal,
                    IntPtr.Zero);

                if (testHandle.IsInvalid) continue;

                var attrs = new Windows.HidAttributes();
                if (Windows.NativeMethods.HidD_GetAttributes(testHandle, ref attrs))
                {
                    if ((vendorId == 0 || attrs.VendorId == vendorId) && 
                        (productId == 0 || attrs.ProductId == productId))
                    {
                        devices.Add(new HidDeviceInfo
                        {
                            DevicePath = devicePath,
                            VendorId = attrs.VendorId,
                            ProductId = attrs.ProductId
                        });
                    }
                }
            }
            finally
            {
                Marshal.FreeHGlobal(detailDataBuffer);
            }
        }

        return devices;
    }

    [System.Runtime.Versioning.SupportedOSPlatform("linux")]
    private static IEnumerable<HidDeviceInfo> EnumerateLinuxDevices(ushort vendorId, ushort productId)
    {
        var devices = new List<HidDeviceInfo>();

        for (int i = 0; i < 32; i++)
        {
            string path = $"/dev/hidraw{i}";
            
            if (!File.Exists(path))
                continue;

            int fd = Linux.NativeMethods.open(path, Linux.NativeMethods.O_RDONLY);
            if (fd < 0)
                continue;

            try
            {
                var devInfo = new Linux.NativeMethods.HidrawDevinfo();
                if (Linux.NativeMethods.ioctl(fd, Linux.NativeMethods.HidioCgrawinfo, ref devInfo) >= 0)
                {
                    ushort vid = (ushort)(devInfo.Vendor & 0xFFFF);
                    ushort pid = (ushort)(devInfo.Product & 0xFFFF);

                    if ((vendorId == 0 || vid == vendorId) && 
                        (productId == 0 || pid == productId))
                    {
                        devices.Add(new HidDeviceInfo
                        {
                            DevicePath = path,
                            VendorId = vid,
                            ProductId = pid
                        });
                    }
                }
            }
            finally
            {
                Linux.NativeMethods.close(fd);
            }
        }

        return devices;
    }
}
