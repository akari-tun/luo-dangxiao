using System.Runtime.InteropServices;
using System.Runtime.Versioning;
using Microsoft.Win32.SafeHandles;

namespace luo.dangxiao.cardreader.Yc.Platform.Windows;

[SupportedOSPlatform("windows")]
internal static partial class NativeMethods
{
    private const string HidLib = "hid.dll";
    private const string SetupApiLib = "setupapi.dll";
    private const string Kernel32Lib = "kernel32.dll";

    [DllImport(HidLib, SetLastError = false)]
    internal static extern void HidD_GetHidGuid(out Guid hidGuid);

    [DllImport(HidLib, SetLastError = false)]
    [return: MarshalAs(UnmanagedType.Bool)]
    internal static extern bool HidD_GetAttributes(SafeFileHandle hidDeviceObject, ref HidAttributes attributes);

    [DllImport(HidLib, SetLastError = true, CharSet = CharSet.Auto)]
    [return: MarshalAs(UnmanagedType.Bool)]
    internal static extern bool HidD_GetManufacturerString(SafeFileHandle hidDeviceObject, IntPtr buffer, uint bufferLength);

    [DllImport(HidLib, SetLastError = true, CharSet = CharSet.Auto)]
    [return: MarshalAs(UnmanagedType.Bool)]
    internal static extern bool HidD_GetProductString(SafeFileHandle hidDeviceObject, IntPtr buffer, uint bufferLength);

    [DllImport(HidLib, SetLastError = true, CharSet = CharSet.Auto)]
    [return: MarshalAs(UnmanagedType.Bool)]
    internal static extern bool HidD_GetSerialNumberString(SafeFileHandle hidDeviceObject, IntPtr buffer, uint bufferLength);

    [DllImport(HidLib, SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)]
    internal static extern bool HidD_FlushQueue(SafeFileHandle hidDeviceObject);

    [DllImport(SetupApiLib, SetLastError = true, CharSet = CharSet.Unicode)]
    internal static extern SafeDeviceInfoSetHandle SetupDiGetClassDevs(
        ref Guid classGuid,
        string? enumerator,
        IntPtr hwndParent,
        uint flags);

    [DllImport(SetupApiLib, SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)]
    internal static extern bool SetupDiEnumDeviceInterfaces(
        SafeDeviceInfoSetHandle deviceInfoSet,
        IntPtr deviceInfoData,
        in Guid interfaceClassGuid,
        uint memberIndex,
        ref SpDeviceInterfaceData deviceInterfaceData);

    [DllImport(SetupApiLib, SetLastError = true, CharSet = CharSet.Auto)]
    [return: MarshalAs(UnmanagedType.Bool)]
    internal static extern bool SetupDiGetDeviceInterfaceDetail(
        SafeDeviceInfoSetHandle deviceInfoSet,
        ref SpDeviceInterfaceData deviceInterfaceData,
        IntPtr deviceInterfaceDetailData,
        uint deviceInterfaceDetailDataSize,
        out uint requiredSize,
        IntPtr deviceInfoData);

    [DllImport(SetupApiLib, SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)]
    internal static extern bool SetupDiDestroyDeviceInfoList(IntPtr deviceInfoSet);

    [DllImport(Kernel32Lib, SetLastError = true, CharSet = CharSet.Unicode, EntryPoint = "CreateFileW")]
    internal static extern SafeFileHandle CreateFile(
        string lpFileName,
        uint dwDesiredAccess,
        uint dwShareMode,
        SafeFileHandle? lpSecurityAttributes,
        uint dwCreationDisposition,
        uint dwFlagsAndAttributes,
        SafeFileHandle? hTemplateFile);

    [DllImport(Kernel32Lib, SetLastError = true, CharSet = CharSet.Unicode, EntryPoint = "CreateFileW")]
    internal static extern SafeFileHandle CreateFile(
        string lpFileName,
        uint dwDesiredAccess,
        uint dwShareMode,
        IntPtr lpSecurityAttributes,
        uint dwCreationDisposition,
        uint dwFlagsAndAttributes,
        IntPtr hTemplateFile);

    [DllImport(Kernel32Lib, SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)]
    internal static extern bool ReadFile(
        SafeFileHandle hFile,
        byte[] lpBuffer,
        uint nNumberOfBytesToRead,
        out uint lpNumberOfBytesRead,
        IntPtr lpOverlapped);

    [DllImport(Kernel32Lib, SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)]
    internal static extern bool WriteFile(
        SafeFileHandle hFile,
        byte[] lpBuffer,
        uint nNumberOfBytesToWrite,
        out uint lpNumberOfBytesWritten,
        IntPtr lpOverlapped);

    [DllImport(Kernel32Lib, SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)]
    internal static extern bool CancelIo(SafeFileHandle hFile);

    internal static class FileAccess
    {
        internal const uint GenericRead = 0x80000000;
        internal const uint GenericWrite = 0x40000000;
        internal const uint GenericReadWrite = GenericRead | GenericWrite;
    }

    internal static class FileShare
    {
        internal const uint Read = 0x00000001;
        internal const uint Write = 0x00000002;
        internal const uint ReadWrite = Read | Write;
    }

    internal static class CreationDisposition
    {
        internal const uint OpenExisting = 3;
    }

    internal static class FileAttributes
    {
        internal const uint Normal = 0x00000080;
        internal const uint Overlapped = 0x40000000;
    }

    internal static class SetupApiFlags
    {
        internal const uint DigcfPresent = 0x00000002;
        internal const uint DigcfDeviceInterface = 0x00000010;
    }
}

[StructLayout(LayoutKind.Sequential)]
internal struct HidAttributes
{
    public uint Size;
    public ushort VendorId;
    public ushort ProductId;
    public ushort VersionNumber;

    public HidAttributes()
    {
        Size = (uint)Marshal.SizeOf<HidAttributes>();
    }
}

[StructLayout(LayoutKind.Sequential)]
internal struct SpDeviceInterfaceData
{
    public int cbSize;
    public Guid InterfaceClassGuid;
    public uint Flags;
    private UIntPtr Reserved;

    public SpDeviceInterfaceData()
    {
        cbSize = Marshal.SizeOf<SpDeviceInterfaceData>();
    }
}

[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
internal struct SpDeviceInterfaceDetailData
{
    public int cbSize;
    [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
    public string DevicePath;

    public SpDeviceInterfaceDetailData()
    {
        cbSize = IntPtr.Size == 8 ? 8 : 6;
        DevicePath = string.Empty;
    }
}

[SupportedOSPlatform("windows")]
internal sealed class SafeDeviceInfoSetHandle : SafeHandleZeroOrMinusOneIsInvalid
{
    public SafeDeviceInfoSetHandle() : base(true) { }

    protected override bool ReleaseHandle()
    {
        return NativeMethods.SetupDiDestroyDeviceInfoList(handle);
    }
}
