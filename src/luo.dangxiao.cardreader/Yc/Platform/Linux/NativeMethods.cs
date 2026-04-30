using System.Runtime.InteropServices;
using System.Runtime.Versioning;

namespace luo.dangxiao.cardreader.Yc.Platform.Linux;

[SupportedOSPlatform("linux")]
internal static partial class NativeMethods
{
    private const string Libc = "libc";

    internal const int O_RDONLY = 0;
    internal const int O_WRONLY = 1;
    internal const int O_RDWR = 2;
    internal const int O_NONBLOCK = 0x800;

    internal const uint HidioCgrawinfo = 0x80084803;
    internal const uint HidioCgrdescsize = 0x80044801;
    internal const uint HidioCgrdesc = 0x90044802;

    [DllImport(Libc, SetLastError = true)]
    internal static extern int open(string pathname, int flags);

    [DllImport(Libc, SetLastError = true)]
    internal static extern int close(int fd);

    [DllImport(Libc, SetLastError = true)]
    internal static extern nint read(int fd, byte[] buf, nint count);

    [DllImport(Libc, SetLastError = true)]
    internal static extern nint write(int fd, byte[] buf, nint count);

    [DllImport(Libc, SetLastError = true)]
    internal static extern int ioctl(int fd, uint request, ref HidrawDevinfo data);

    [DllImport(Libc, SetLastError = true)]
    internal static extern int ioctl(int fd, uint request, ref HidrawReportDescriptor data);

    [DllImport(Libc, SetLastError = true)]
    internal static extern int ioctl(int fd, uint request, ref int value);

    [StructLayout(LayoutKind.Sequential)]
    internal struct HidrawDevinfo
    {
        public uint Bustype;
        public short Vendor;
        public short Product;
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct HidrawReportDescriptor
    {
        public uint Size;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4096)]
        public byte[] Value;

        public HidrawReportDescriptor()
        {
            Value = new byte[4096];
        }
    }
}
