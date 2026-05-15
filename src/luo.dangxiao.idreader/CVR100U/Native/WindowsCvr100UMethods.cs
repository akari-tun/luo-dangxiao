using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;

namespace luo.dangxiao.idreader.CVR100U.Native;

/// <summary>
/// Windows-specific base for CVR100U native methods implementing x86/x64 DLL resolution.
/// </summary>
public abstract class WindowsCvr100UMethodsBase : ICvr100UMethods
{
    static WindowsCvr100UMethodsBase()
    {
        NativeLibrary.SetDllImportResolver(typeof(WindowsCvr100UMethodsBase).Assembly, ResolveWindowsDll);
    }

    private static IntPtr ResolveWindowsDll(string libraryName, Assembly assembly, DllImportSearchPath? searchPath)
    {
        if (libraryName != "Termb.dll")
        {
            return IntPtr.Zero;
        }

        var arch = RuntimeInformation.ProcessArchitecture;
        var basePath = AppContext.BaseDirectory;

        var dllDir = arch switch
        {
            Architecture.X86 => Path.Combine(basePath, "CVR100U", "libs", "win-x86"),
            Architecture.X64 => Path.Combine(basePath, "CVR100U", "libs", "win-x64"),
            Architecture.Arm64 => Path.Combine(basePath, "CVR100U", "libs", "win-arm64"),
            _ => basePath,
        };

        var dllPath = Path.Combine(dllDir, "Termb.dll");
        return NativeLibrary.TryLoad(dllPath, out var handle) ? handle : IntPtr.Zero;
    }

    public int CVR_InitComm(string? path, int protocolType)
    {
        throw new PlatformNotSupportedException("CVR_InitComm(string, int) is not supported on Windows.");
    }

    public int CVR_InitComm(int port) => CVR_InitComm_Native(port);
    public int CVR_CloseComm() => CVR_CloseComm_Native();
    public int CVR_Authenticate() => CVR_Authenticate_Native();
    public int CVR_Read_Content(int active) => CVR_Read_Content_Native(active);
    public int CVR_GetSAMID(StringBuilder buffer, ref int length) => CVR_GetSAMID_Native(buffer, ref length);
    public int CVR_GetUID(byte[] buffer, ref int length) => throw new PlatformNotSupportedException("CVR_GetUID is not available on Windows SDK.");
    public int GetPeopleName(StringBuilder buffer, ref int length) => GetPeopleName_Native(buffer, ref length);
    public int GetPeopleSex(StringBuilder buffer, ref int length) => GetPeopleSex_Native(buffer, ref length);
    public int GetPeopleNation(StringBuilder buffer, ref int length) => GetPeopleNation_Native(buffer, ref length);
    public int GetPeopleBirthday(StringBuilder buffer, ref int length) => GetPeopleBirthday_Native(buffer, ref length);
    public int GetPeopleIDCode(StringBuilder buffer, ref int length) => GetPeopleIDCode_Native(buffer, ref length);
    public int GetDepartment(StringBuilder buffer, ref int length) => GetDepartment_Native(buffer, ref length);
    public int GetStartDate(StringBuilder buffer, ref int length) => GetStartDate_Native(buffer, ref length);
    public int GetEndDate(StringBuilder buffer, ref int length) => GetEndDate_Native(buffer, ref length);
    public int GetPeopleAddress(StringBuilder buffer, ref int length) => GetPeopleAddress_Native(buffer, ref length);
    public int GetBMPData(byte[] buffer, ref int length) => GetBMPData_Native(buffer, ref length);
    public int GetCertType(byte[] buffer, ref int length) => GetCertType_Native(buffer, ref length);

    [DllImport("Termb.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "CVR_InitComm")]
    private static extern int CVR_InitComm_Native(int port);

    [DllImport("Termb.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "CVR_CloseComm")]
    private static extern int CVR_CloseComm_Native();

    [DllImport("Termb.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "CVR_Authenticate")]
    private static extern int CVR_Authenticate_Native();

    [DllImport("Termb.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "CVR_Read_Content")]
    private static extern int CVR_Read_Content_Native(int active);

    [DllImport("Termb.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "CVR_GetSAMID")]
    private static extern int CVR_GetSAMID_Native(StringBuilder buffer, ref int length);

    [DllImport("Termb.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "GetPeopleName")]
    private static extern int GetPeopleName_Native(StringBuilder buffer, ref int length);

    [DllImport("Termb.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "GetPeopleSex")]
    private static extern int GetPeopleSex_Native(StringBuilder buffer, ref int length);

    [DllImport("Termb.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "GetPeopleNation")]
    private static extern int GetPeopleNation_Native(StringBuilder buffer, ref int length);

    [DllImport("Termb.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "GetPeopleBirthday")]
    private static extern int GetPeopleBirthday_Native(StringBuilder buffer, ref int length);

    [DllImport("Termb.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "GetPeopleIDCode")]
    private static extern int GetPeopleIDCode_Native(StringBuilder buffer, ref int length);

    [DllImport("Termb.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "GetDepartment")]
    private static extern int GetDepartment_Native(StringBuilder buffer, ref int length);

    [DllImport("Termb.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "GetStartDate")]
    private static extern int GetStartDate_Native(StringBuilder buffer, ref int length);

    [DllImport("Termb.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "GetEndDate")]
    private static extern int GetEndDate_Native(StringBuilder buffer, ref int length);

    [DllImport("Termb.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "GetPeopleAddress")]
    private static extern int GetPeopleAddress_Native(StringBuilder buffer, ref int length);

    [DllImport("Termb.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "GetBMPData")]
    private static extern int GetBMPData_Native(byte[] buffer, ref int length);

    [DllImport("Termb.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "GetCertType")]
    private static extern int GetCertType_Native(byte[] buffer, ref int length);
}

/// <summary>
/// Windows x86 (32-bit) specific implementation.
/// </summary>
public sealed class WindowsX86Cvr100UMethods : WindowsCvr100UMethodsBase
{
}

/// <summary>
/// Windows x64 (64-bit) specific implementation.
/// </summary>
public sealed class WindowsX64Cvr100UMethods : WindowsCvr100UMethodsBase
{
}
