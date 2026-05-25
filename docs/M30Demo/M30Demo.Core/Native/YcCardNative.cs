using System;
using System.Runtime.InteropServices;
using System.Text;

namespace M30Demo.Core.Native;

/// <summary>
/// P/Invoke declarations for YCCard.DLL
/// Mirrors docs/YC.SelfCardSys.CardOperate/YCCard.cs
/// </summary>
internal static class YcCardNative
{
    private const string Dll = "yccard.dll";

    [DllImport(Dll, EntryPoint = "OpenComm")]
    public static extern IntPtr OpenComm(byte commPort);

    [DllImport(Dll, EntryPoint = "CloseComm")]
    public static extern int CloseComm(IntPtr hwnd);

    [DllImport(Dll, EntryPoint = "ReadCard_ID")]
    public static extern int ReadCard_ID(IntPtr hwnd, ref int tagType, ref uint cardID);

    [DllImport(Dll, EntryPoint = "ReadCard_ID_NEW")]
    public static extern int ReadCard_ID_NEW(IntPtr hwnd, ref int tagType, ref int cardID);

    [DllImport(Dll, EntryPoint = "Query_Card_Type")]
    public static extern int Query_Card_Type(
        IntPtr hwnd,
        ref int sysType,
        ref int cardType,
        ref int intsalesOperatorId,
        ref uint cardserno,
        int waitTime,
        ref byte secretKey);

    [DllImport(Dll, EntryPoint = "Query_Pos_UserCard12")]
    public static extern int Query_Pos_UserCard12(
        IntPtr hwnd,
        ref int cardType,
        ref int optNum,
        ref int cardID,
        StringBuilder userNo,
        ref int userType,
        ref uint cardserno,
        ref int mlngChkSum1,
        ref int value1,
        ref int lastPay1,
        ref int count1,
        ref int consumeAdd1,
        ref int chkSum2,
        ref int value2,
        ref int lastPay2,
        ref int count2,
        ref int consumeAdd2,
        ref int useTerm,
        ref int addCount,
        int waitTime,
        ref byte secretKey);

    [DllImport(Dll, EntryPoint = "Init_Pos_UserCard12")]
    public static extern int Init_Pos_UserCard12(
        IntPtr hwnd,
        int cardID,
        string cardno,
        int usertype,
        int waitTime,
        ref uint cardserno,
        int useTerm,
        ref byte secretKey);

    [DllImport(Dll, EntryPoint = "ActivatePosUserCard12")]
    public static extern int ActivatePosUserCard12(
        IntPtr hwnd,
        int cardID,
        byte lockFlag,
        byte newCardFlag,
        byte mainCardFlag,
        int userType,
        ref uint cardserno,
        int useTerm,
        ref byte secretKey);

    [DllImport(Dll, EntryPoint = "RST_Pos_UserCard12")]
    public static extern int RST_Pos_UserCard12(
        IntPtr hwnd,
        uint cardserno,
        int waitime,
        ref byte secretKey);

    [DllImport(Dll, EntryPoint = "WRT_Pos_UserCard_AddCount12")]
    public static extern int WRT_Pos_UserCard_AddCount12(
        IntPtr hwnd,
        int value,
        uint cardserno,
        int waitime);

    [DllImport(Dll, EntryPoint = "Init_Js_UserCard")]
    public static extern int Init_Js_UserCard(
        IntPtr hwnd,
        int cardID,
        string cardNo,
        int userType,
        ref uint cardserno,
        ref byte secretKey);

    [DllImport(Dll, EntryPoint = "Init_Js_UserCard_N")]
    public static extern int Init_Js_UserCard_N(
        IntPtr hwnd,
        int cardID,
        string cardNo,
        int userType,
        int cardBalance,
        int chargeTimes,
        ref int cardserno,
        ref byte secretKey);

    [DllImport(Dll, EntryPoint = "RSTJsUserCard")]
    public static extern int RSTJsUserCard(
        IntPtr hwnd,
        uint cardserno,
        ref byte secretKey);

    [DllImport(Dll, EntryPoint = "QueryJsCard")]
    public static extern int QueryJsCard(
        IntPtr hwnd,
        ref int cardType,
        ref int optNum,
        ref int cardNo,
        StringBuilder userPwd,
        ref int cardserno,
        ref int value,
        ref int count,
        ref int userType,
        int waitime,
        ref byte secretKey);

    [DllImport(Dll, EntryPoint = "WRT_Js_UserCard_AddCount")]
    public static extern int WRT_Js_UserCard_AddCount(
        IntPtr hwnd,
        int balance,
        StringBuilder chargeDateTime,
        uint cardserno);

    [DllImport(Dll, EntryPoint = "rf_beep")]
    public static extern int rf_beep(IntPtr hwnd, int msec);

    [DllImport(Dll, EntryPoint = "Init_Pos_OPTCard12")]
    public static extern int Init_Pos_OPTCard12(
        IntPtr hwnd,
        int optNum,
        int waitTime,
        ref int cardserno,
        ref byte secretKey);

    [DllImport(Dll, EntryPoint = "RST_Pos_OPTCard12")]
    public static extern int RST_Pos_OPTCard12(
        IntPtr hwnd,
        int cardserno,
        int waitime,
        ref byte secretKey);

    [DllImport(Dll, EntryPoint = "Change_Pos_UserType12_NEW")]
    public static extern int Change_Pos_UserType12_NEW(
        IntPtr hwnd,
        ref int oldUserType,
        int userType,
        int sysType);

    [DllImport(Dll, EntryPoint = "WRT_UserCard_Term")]
    public static extern int WRT_UserCard_Term(
        IntPtr hwnd,
        int sysType,
        int useTerm,
        int cardserno,
        int waitTime);

    [DllImport(Dll, EntryPoint = "Make_UserCard_New091102")]
    public static extern int Make_UserCard_New091102(
        IntPtr hwnd,
        int value,
        int sysType,
        int cardSerno,
        ref int cardValue1,
        ref int cardValue2,
        ref int cardValue,
        int waitTime);

    [DllImport(Dll, EntryPoint = "SDFZ_CalculateDynamicKey")]
    public static extern int SDFZ_CalculateDynamicKey(
        uint cardID,
        ref byte baseKey,
        ref byte passWords,
        byte encryptMode);

    [DllImport(Dll, EntryPoint = "SDFZ_GetSectorNumber")]
    public static extern int SDFZ_GetSectorNumber(ref uint xfSector, ref uint jsSector);

    [DllImport(Dll, EntryPoint = "SDFZ_InitPosUserCard12")]
    public static extern int SDFZ_InitPosUserCard12(
        uint serno,
        int cardID,
        string userNumber,
        int cardType,
        int useTerm,
        ref byte userCodeNew,
        ref byte dataBuff);

    [DllImport(Dll, EntryPoint = "SDFZ_WalletReCharge")]
    public static extern int SDFZ_WalletReCharge(int value, ref byte inData, ref byte outData);

    [DllImport(Dll, EntryPoint = "SDFZ_RecycleUserCard12")]
    public static extern int SDFZ_RecycleUserCard12(ref byte initBuff);
}
