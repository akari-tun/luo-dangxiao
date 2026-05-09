using System.Runtime.InteropServices;
using System.Text;

namespace luo.dangxiao.idreader.CVR100U.Native;

/// <summary>
/// Linux-specific implementation of CVR100U native methods.
/// Uses lib100UD.so with Cdecl calling convention.
/// </summary>
public sealed class LinuxCvr100UMethods : ICvr100UMethods
{
    public int CVR_InitComm(string? path, int protocolType)
    {
        return CVR_InitComm_Native(path, protocolType);
    }

    public int CVR_InitComm(int port)
    {
        throw new PlatformNotSupportedException("CVR_InitComm(int port) is only supported on Windows.");
    }

    public int CVR_CloseComm() => CVR_CloseComm_Native();

    public int CVR_Authenticate() => CVR_AuthenticateForNoJudge();

    public int CVR_Read_Content(int active) => CVR_Read_Content_Native(active);

    public int CVR_GetSAMID(StringBuilder buffer, ref int length) => CVR_GetSAMID_Native(buffer, ref length);

    public int CVR_GetUID(byte[] buffer, ref int length) => CVR_GetUID_Native(buffer, ref length);

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

    [DllImport("lib100UD.so", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, EntryPoint = "CVR_InitComm")]
    private static extern int CVR_InitComm_Native(string? path, int protocolType);

    [DllImport("lib100UD.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "CVR_CloseComm")]
    private static extern int CVR_CloseComm_Native();

    [DllImport("lib100UD.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "CVR_AuthenticateForNoJudge")]
    private static extern int CVR_AuthenticateForNoJudge();

    [DllImport("lib100UD.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "CVR_Read_Content")]
    private static extern int CVR_Read_Content_Native(int active);

    [DllImport("lib100UD.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "CVR_GetSAMID")]
    private static extern int CVR_GetSAMID_Native(StringBuilder buffer, ref int length);

    [DllImport("lib100UD.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "CVR_GetUID")]
    private static extern int CVR_GetUID_Native(byte[] buffer, ref int length);

    [DllImport("lib100UD.so", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, EntryPoint = "GetPeopleName")]
    private static extern int GetPeopleName_Native(StringBuilder buffer, ref int length);

    [DllImport("lib100UD.so", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, EntryPoint = "GetPeopleSex")]
    private static extern int GetPeopleSex_Native(StringBuilder buffer, ref int length);

    [DllImport("lib100UD.so", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, EntryPoint = "GetPeopleNation")]
    private static extern int GetPeopleNation_Native(StringBuilder buffer, ref int length);

    [DllImport("lib100UD.so", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, EntryPoint = "GetPeopleBirthday")]
    private static extern int GetPeopleBirthday_Native(StringBuilder buffer, ref int length);

    [DllImport("lib100UD.so", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, EntryPoint = "GetPeopleIDCode")]
    private static extern int GetPeopleIDCode_Native(StringBuilder buffer, ref int length);

    [DllImport("lib100UD.so", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, EntryPoint = "GetDepartment")]
    private static extern int GetDepartment_Native(StringBuilder buffer, ref int length);

    [DllImport("lib100UD.so", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, EntryPoint = "GetStartDate")]
    private static extern int GetStartDate_Native(StringBuilder buffer, ref int length);

    [DllImport("lib100UD.so", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, EntryPoint = "GetEndDate")]
    private static extern int GetEndDate_Native(StringBuilder buffer, ref int length);

    [DllImport("lib100UD.so", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, EntryPoint = "GetPeopleAddress")]
    private static extern int GetPeopleAddress_Native(StringBuilder buffer, ref int length);

    [DllImport("lib100UD.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GetBMPData")]
    private static extern int GetBMPData_Native(byte[] buffer, ref int length);

    [DllImport("lib100UD.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GetCertType")]
    private static extern int GetCertType_Native(byte[] buffer, ref int length);
}
