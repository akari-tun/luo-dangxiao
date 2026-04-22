using System;
using System.Runtime.InteropServices;

namespace luo.dangxiao.printer.Seaory.Native
{
    public static class SeaorySdk
    {
        private static readonly string LibraryName = RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
            ? "SeaorySDK.dll"
            : "libSeaorySDK.so";

        private static readonly CallingConvention CallConv = RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
            ? CallingConvention.StdCall
            : CallingConvention.Cdecl;

        #region SDK Version

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_SetLogLevel")]
        private static extern void SOY_PR_SetLogLevel_Win(uint dwLogLevel);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_SetLogLevel")]
        private static extern void SOY_PR_SetLogLevel_Linux(uint dwLogLevel);

        public static void SOY_PR_SetLogLevel(uint dwLogLevel)
        {
            if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
                SOY_PR_SetLogLevel_Win(dwLogLevel);
            else
                SOY_PR_SetLogLevel_Linux(dwLogLevel);
        }

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_SdkVersionA")]
        private static extern uint SOY_PR_SdkVersionA_Win([Out] byte[] szVersionOutA);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_SdkVersionA")]
        private static extern uint SOY_PR_SdkVersionA_Linux([Out] byte[] szVersionOutA);

        public static uint SOY_PR_SdkVersionA([Out] byte[] szVersionOutA)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_SdkVersionA_Win(szVersionOutA)
                : SOY_PR_SdkVersionA_Linux(szVersionOutA);
        }

        #endregion

        #region Printer Status and Info

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_GetPrinterStatusA")]
        private static extern uint SOY_PR_GetPrinterStatusA_Win([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, ref uint lpdwStatus);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_GetPrinterStatusA")]
        private static extern uint SOY_PR_GetPrinterStatusA_Linux([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, ref uint lpdwStatus);

        public static uint SOY_PR_GetPrinterStatusA([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, ref uint lpdwStatus)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_GetPrinterStatusA_Win(szPrinterNameA, ref lpdwStatus)
                : SOY_PR_GetPrinterStatusA_Linux(szPrinterNameA, ref lpdwStatus);
        }

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_GetPrinterWarningA")]
        private static extern uint SOY_PR_GetPrinterWarningA_Win([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, ref uint lpdwStatus);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_GetPrinterWarningA")]
        private static extern uint SOY_PR_GetPrinterWarningA_Linux([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, ref uint lpdwStatus);

        public static uint SOY_PR_GetPrinterWarningA([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, ref uint lpdwStatus)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_GetPrinterWarningA_Win(szPrinterNameA, ref lpdwStatus)
                : SOY_PR_GetPrinterWarningA_Linux(szPrinterNameA, ref lpdwStatus);
        }

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_GetPrinterInfoA")]
        private static extern uint SOY_PR_GetPrinterInfoA_Win([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, int nInfoType, [Out] byte[] szInfoOutA);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_GetPrinterInfoA")]
        private static extern uint SOY_PR_GetPrinterInfoA_Linux([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, int nInfoType, [Out] byte[] szInfoOutA);

        public static uint SOY_PR_GetPrinterInfoA([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, int nInfoType, [Out] byte[] szInfoOutA)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_GetPrinterInfoA_Win(szPrinterNameA, nInfoType, szInfoOutA)
                : SOY_PR_GetPrinterInfoA_Linux(szPrinterNameA, nInfoType, szInfoOutA);
        }

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_WaitPrinterBusyA")]
        private static extern uint SOY_PR_WaitPrinterBusyA_Win([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, ref uint lpdwStatus);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_WaitPrinterBusyA")]
        private static extern uint SOY_PR_WaitPrinterBusyA_Linux([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, ref uint lpdwStatus);

        public static uint SOY_PR_WaitPrinterBusyA([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, ref uint lpdwStatus)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_WaitPrinterBusyA_Win(szPrinterNameA, ref lpdwStatus)
                : SOY_PR_WaitPrinterBusyA_Linux(szPrinterNameA, ref lpdwStatus);
        }

        #endregion

        #region Printer Commands

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_ExecCommandA")]
        private static extern uint SOY_PR_ExecCommandA_Win([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, uint dwCommand);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_ExecCommandA")]
        private static extern uint SOY_PR_ExecCommandA_Linux([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, uint dwCommand);

        public static uint SOY_PR_ExecCommandA([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, uint dwCommand)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_ExecCommandA_Win(szPrinterNameA, dwCommand)
                : SOY_PR_ExecCommandA_Linux(szPrinterNameA, dwCommand);
        }

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_ExecCommand2A")]
        private static extern uint SOY_PR_ExecCommand2A_Win([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, uint dwCommand, uint dwPara1, uint dwPara2);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_ExecCommand2A")]
        private static extern uint SOY_PR_ExecCommand2A_Linux([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, uint dwCommand, uint dwPara1, uint dwPara2);

        public static uint SOY_PR_ExecCommand2A([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, uint dwCommand, uint dwPara1, uint dwPara2)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_ExecCommand2A_Win(szPrinterNameA, dwCommand, dwPara1, dwPara2)
                : SOY_PR_ExecCommand2A_Linux(szPrinterNameA, dwCommand, dwPara1, dwPara2);
        }

        #endregion

        #region Printer Configuration

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_GetPrinterConfigA")]
        private static extern uint SOY_PR_GetPrinterConfigA_Win([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, uint dwConfigType, ref int lpnConfigValue);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_GetPrinterConfigA")]
        private static extern uint SOY_PR_GetPrinterConfigA_Linux([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, uint dwConfigType, ref int lpnConfigValue);

        public static uint SOY_PR_GetPrinterConfigA([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, uint dwConfigType, ref int lpnConfigValue)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_GetPrinterConfigA_Win(szPrinterNameA, dwConfigType, ref lpnConfigValue)
                : SOY_PR_GetPrinterConfigA_Linux(szPrinterNameA, dwConfigType, ref lpnConfigValue);
        }

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_SetPrinterConfigA")]
        private static extern uint SOY_PR_SetPrinterConfigA_Win([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, uint dwConfigType, int nConfigValue);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_SetPrinterConfigA")]
        private static extern uint SOY_PR_SetPrinterConfigA_Linux([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, uint dwConfigType, int nConfigValue);

        public static uint SOY_PR_SetPrinterConfigA([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, uint dwConfigType, int nConfigValue)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_SetPrinterConfigA_Win(szPrinterNameA, dwConfigType, nConfigValue)
                : SOY_PR_SetPrinterConfigA_Linux(szPrinterNameA, dwConfigType, nConfigValue);
        }

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_SetStandbyParametersA")]
        private static extern uint SOY_PR_SetStandbyParametersA_Win([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, byte byOutputBin, byte byStandbyPos, uint dwStandbyTime);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_SetStandbyParametersA")]
        private static extern uint SOY_PR_SetStandbyParametersA_Linux([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, byte byOutputBin, byte byStandbyPos, uint dwStandbyTime);

        public static uint SOY_PR_SetStandbyParametersA([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, byte byOutputBin, byte byStandbyPos, uint dwStandbyTime)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_SetStandbyParametersA_Win(szPrinterNameA, byOutputBin, byStandbyPos, dwStandbyTime)
                : SOY_PR_SetStandbyParametersA_Linux(szPrinterNameA, byOutputBin, byStandbyPos, dwStandbyTime);
        }

        #endregion

        #region Simple Print Functions

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_StartPrinting2A")]
        private static extern uint SOY_PR_StartPrinting2A_Win([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, IntPtr lpDocPropIn, ref IntPtr lphPrinterDC);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_StartPrinting2A")]
        private static extern uint SOY_PR_StartPrinting2A_Linux([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, IntPtr lpDocPropIn, ref IntPtr lphPrinterDC);

        public static uint SOY_PR_StartPrinting2A([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, IntPtr lpDocPropIn, ref IntPtr lphPrinterDC)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_StartPrinting2A_Win(szPrinterNameA, lpDocPropIn, ref lphPrinterDC)
                : SOY_PR_StartPrinting2A_Linux(szPrinterNameA, lpDocPropIn, ref lphPrinterDC);
        }

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_StartPage2")]
        private static extern uint SOY_PR_StartPage2_Win(IntPtr hPrinterDC);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_StartPage2")]
        private static extern uint SOY_PR_StartPage2_Linux(IntPtr hPrinterDC);

        public static uint SOY_PR_StartPage2(IntPtr hPrinterDC)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_StartPage2_Win(hPrinterDC)
                : SOY_PR_StartPage2_Linux(hPrinterDC);
        }

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_EndPage2")]
        private static extern uint SOY_PR_EndPage2_Win(IntPtr hPrinterDC);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_EndPage2")]
        private static extern uint SOY_PR_EndPage2_Linux(IntPtr hPrinterDC);

        public static uint SOY_PR_EndPage2(IntPtr hPrinterDC)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_EndPage2_Win(hPrinterDC)
                : SOY_PR_EndPage2_Linux(hPrinterDC);
        }

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_EndPrinting2")]
        private static extern uint SOY_PR_EndPrinting2_Win(IntPtr hPrinterDC, [MarshalAs(UnmanagedType.I1)] bool bCancelJob);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_EndPrinting2")]
        private static extern uint SOY_PR_EndPrinting2_Linux(IntPtr hPrinterDC, [MarshalAs(UnmanagedType.I1)] bool bCancelJob);

        public static uint SOY_PR_EndPrinting2(IntPtr hPrinterDC, [MarshalAs(UnmanagedType.I1)] bool bCancelJob)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_EndPrinting2_Win(hPrinterDC, bCancelJob)
                : SOY_PR_EndPrinting2_Linux(hPrinterDC, bCancelJob);
        }

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_PrintImage2A")]
        private static extern uint SOY_PR_PrintImage2A_Win(IntPtr hPrinterDC, int nX, int nY, int nWidth, int nHeight, [MarshalAs(UnmanagedType.LPStr)] string szImageUrlA);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_PrintImage2A")]
        private static extern uint SOY_PR_PrintImage2A_Linux(IntPtr hPrinterDC, int nX, int nY, int nWidth, int nHeight, [MarshalAs(UnmanagedType.LPStr)] string szImageUrlA);

        public static uint SOY_PR_PrintImage2A(IntPtr hPrinterDC, int nX, int nY, int nWidth, int nHeight, [MarshalAs(UnmanagedType.LPStr)] string szImageUrlA)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_PrintImage2A_Win(hPrinterDC, nX, nY, nWidth, nHeight, szImageUrlA)
                : SOY_PR_PrintImage2A_Linux(hPrinterDC, nX, nY, nWidth, nHeight, szImageUrlA);
        }

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_PrintText2A")]
        private static extern uint SOY_PR_PrintText2A_Win(
            IntPtr hPrinterDC,
            int nX,
            int nY,
            [MarshalAs(UnmanagedType.LPStr)] string szTextA,
            [MarshalAs(UnmanagedType.LPStr)] string szFontNameA,
            int nFontSize,
            int nFontWeight,
            byte byFontAttribute,
            byte byCharSet,
            uint dwTextColor,
            [MarshalAs(UnmanagedType.I1)] bool bTransparentBack);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_PrintText2A")]
        private static extern uint SOY_PR_PrintText2A_Linux(
            IntPtr hPrinterDC,
            int nX,
            int nY,
            [MarshalAs(UnmanagedType.LPStr)] string szTextA,
            [MarshalAs(UnmanagedType.LPStr)] string szFontNameA,
            int nFontSize,
            int nFontWeight,
            byte byFontAttribute,
            byte byCharSet,
            uint dwTextColor,
            [MarshalAs(UnmanagedType.I1)] bool bTransparentBack);

        public static uint SOY_PR_PrintText2A(
            IntPtr hPrinterDC,
            int nX,
            int nY,
            [MarshalAs(UnmanagedType.LPStr)] string szTextA,
            [MarshalAs(UnmanagedType.LPStr)] string szFontNameA,
            int nFontSize,
            int nFontWeight,
            byte byFontAttribute,
            byte byCharSet,
            uint dwTextColor,
            [MarshalAs(UnmanagedType.I1)] bool bTransparentBack)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_PrintText2A_Win(hPrinterDC, nX, nY, szTextA, szFontNameA, nFontSize, nFontWeight, byFontAttribute, byCharSet, dwTextColor, bTransparentBack)
                : SOY_PR_PrintText2A_Linux(hPrinterDC, nX, nY, szTextA, szFontNameA, nFontSize, nFontWeight, byFontAttribute, byCharSet, dwTextColor, bTransparentBack);
        }

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_PrintText3A")]
        private static extern uint SOY_PR_PrintText3A_Win(
            IntPtr hPrinterDC,
            int nX,
            int nY,
            [MarshalAs(UnmanagedType.LPStr)] string szTextA,
            [MarshalAs(UnmanagedType.LPStr)] string szFontNameA,
            int nFontSize,
            int nFontWeight,
            byte byFontAttribute,
            byte byCharSet,
            uint dwTextColor,
            [MarshalAs(UnmanagedType.I1)] bool bTransparentBack,
            int nExtraSpace);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_PrintText3A")]
        private static extern uint SOY_PR_PrintText3A_Linux(
            IntPtr hPrinterDC,
            int nX,
            int nY,
            [MarshalAs(UnmanagedType.LPStr)] string szTextA,
            [MarshalAs(UnmanagedType.LPStr)] string szFontNameA,
            int nFontSize,
            int nFontWeight,
            byte byFontAttribute,
            byte byCharSet,
            uint dwTextColor,
            [MarshalAs(UnmanagedType.I1)] bool bTransparentBack,
            int nExtraSpace);

        public static uint SOY_PR_PrintText3A(
            IntPtr hPrinterDC,
            int nX,
            int nY,
            [MarshalAs(UnmanagedType.LPStr)] string szTextA,
            [MarshalAs(UnmanagedType.LPStr)] string szFontNameA,
            int nFontSize,
            int nFontWeight,
            byte byFontAttribute,
            byte byCharSet,
            uint dwTextColor,
            [MarshalAs(UnmanagedType.I1)] bool bTransparentBack,
            int nExtraSpace)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_PrintText3A_Win(hPrinterDC, nX, nY, szTextA, szFontNameA, nFontSize, nFontWeight, byFontAttribute, byCharSet, dwTextColor, bTransparentBack, nExtraSpace)
                : SOY_PR_PrintText3A_Linux(hPrinterDC, nX, nY, szTextA, szFontNameA, nFontSize, nFontWeight, byFontAttribute, byCharSet, dwTextColor, bTransparentBack, nExtraSpace);
        }

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_PrintBlackImage2A")]
        private static extern uint SOY_PR_PrintBlackImage2A_Win(
            IntPtr hPrinterDC,
            int nX,
            int nY,
            int nWidth,
            int nHeight,
            [MarshalAs(UnmanagedType.LPStr)] string szImageUrlA,
            float fThreshold);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_PrintBlackImage2A")]
        private static extern uint SOY_PR_PrintBlackImage2A_Linux(
            IntPtr hPrinterDC,
            int nX,
            int nY,
            int nWidth,
            int nHeight,
            [MarshalAs(UnmanagedType.LPStr)] string szImageUrlA,
            float fThreshold);

        public static uint SOY_PR_PrintBlackImage2A(
            IntPtr hPrinterDC,
            int nX,
            int nY,
            int nWidth,
            int nHeight,
            [MarshalAs(UnmanagedType.LPStr)] string szImageUrlA,
            float fThreshold)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_PrintBlackImage2A_Win(hPrinterDC, nX, nY, nWidth, nHeight, szImageUrlA, fThreshold)
                : SOY_PR_PrintBlackImage2A_Linux(hPrinterDC, nX, nY, nWidth, nHeight, szImageUrlA, fThreshold);
        }

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_SetPrintAreaBmp2A")]
        private static extern uint SOY_PR_SetPrintAreaBmp2A_Win(IntPtr hPrinterDC, int nSide, int nPanel, [MarshalAs(UnmanagedType.LPStr)] string szAreaImageA);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_SetPrintAreaBmp2A")]
        private static extern uint SOY_PR_SetPrintAreaBmp2A_Linux(IntPtr hPrinterDC, int nSide, int nPanel, [MarshalAs(UnmanagedType.LPStr)] string szAreaImageA);

        public static uint SOY_PR_SetPrintAreaBmp2A(IntPtr hPrinterDC, int nSide, int nPanel, [MarshalAs(UnmanagedType.LPStr)] string szAreaImageA)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_SetPrintAreaBmp2A_Win(hPrinterDC, nSide, nPanel, szAreaImageA)
                : SOY_PR_SetPrintAreaBmp2A_Linux(hPrinterDC, nSide, nPanel, szAreaImageA);
        }

        #endregion

        #region Direct Card Print (Advanced)

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_PrintOneCardA")]
        private static extern uint SOY_PR_PrintOneCardA_Win([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, ref PrintCardParam lpJobPara);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_PrintOneCardA")]
        private static extern uint SOY_PR_PrintOneCardA_Linux([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, ref PrintCardParam lpJobPara);

        public static uint SOY_PR_PrintOneCardA([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, ref PrintCardParam lpJobPara)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_PrintOneCardA_Win(szPrinterNameA, ref lpJobPara)
                : SOY_PR_PrintOneCardA_Linux(szPrinterNameA, ref lpJobPara);
        }

        #endregion

        #region Magnetic Stripe Operations

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_ReadTrackA")]
        private static extern uint SOY_PR_ReadTrackA_Win(
            [MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA,
            uint dwMode,
            [Out] byte[] szTrack1A,
            [Out] byte[] szTrack2A,
            [Out] byte[] szTrack3A);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_ReadTrackA")]
        private static extern uint SOY_PR_ReadTrackA_Linux(
            [MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA,
            uint dwMode,
            [Out] byte[] szTrack1A,
            [Out] byte[] szTrack2A,
            [Out] byte[] szTrack3A);

        public static uint SOY_PR_ReadTrackA(
            [MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA,
            uint dwMode,
            [Out] byte[] szTrack1A,
            [Out] byte[] szTrack2A,
            [Out] byte[] szTrack3A)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_ReadTrackA_Win(szPrinterNameA, dwMode, szTrack1A, szTrack2A, szTrack3A)
                : SOY_PR_ReadTrackA_Linux(szPrinterNameA, dwMode, szTrack1A, szTrack2A, szTrack3A);
        }

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_WriteTrackA")]
        private static extern uint SOY_PR_WriteTrackA_Win(
            [MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA,
            uint dwMode,
            [MarshalAs(UnmanagedType.LPStr)] string szTrack1A,
            [MarshalAs(UnmanagedType.LPStr)] string szTrack2A,
            [MarshalAs(UnmanagedType.LPStr)] string szTrack3A);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_WriteTrackA")]
        private static extern uint SOY_PR_WriteTrackA_Linux(
            [MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA,
            uint dwMode,
            [MarshalAs(UnmanagedType.LPStr)] string szTrack1A,
            [MarshalAs(UnmanagedType.LPStr)] string szTrack2A,
            [MarshalAs(UnmanagedType.LPStr)] string szTrack3A);

        public static uint SOY_PR_WriteTrackA(
            [MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA,
            uint dwMode,
            [MarshalAs(UnmanagedType.LPStr)] string szTrack1A,
            [MarshalAs(UnmanagedType.LPStr)] string szTrack2A,
            [MarshalAs(UnmanagedType.LPStr)] string szTrack3A)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_WriteTrackA_Win(szPrinterNameA, dwMode, szTrack1A, szTrack2A, szTrack3A)
                : SOY_PR_WriteTrackA_Linux(szPrinterNameA, dwMode, szTrack1A, szTrack2A, szTrack3A);
        }

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_EncodeTrackA")]
        private static extern uint SOY_PR_EncodeTrackA_Win(
            [MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA,
            uint dwMode,
            [MarshalAs(UnmanagedType.LPStr)] string szTrack1A,
            [MarshalAs(UnmanagedType.LPStr)] string szTrack2A,
            [MarshalAs(UnmanagedType.LPStr)] string szTrack3A,
            uint dwRetry);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_EncodeTrackA")]
        private static extern uint SOY_PR_EncodeTrackA_Linux(
            [MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA,
            uint dwMode,
            [MarshalAs(UnmanagedType.LPStr)] string szTrack1A,
            [MarshalAs(UnmanagedType.LPStr)] string szTrack2A,
            [MarshalAs(UnmanagedType.LPStr)] string szTrack3A,
            uint dwRetry);

        public static uint SOY_PR_EncodeTrackA(
            [MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA,
            uint dwMode,
            [MarshalAs(UnmanagedType.LPStr)] string szTrack1A,
            [MarshalAs(UnmanagedType.LPStr)] string szTrack2A,
            [MarshalAs(UnmanagedType.LPStr)] string szTrack3A,
            uint dwRetry)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_EncodeTrackA_Win(szPrinterNameA, dwMode, szTrack1A, szTrack2A, szTrack3A, dwRetry)
                : SOY_PR_EncodeTrackA_Linux(szPrinterNameA, dwMode, szTrack1A, szTrack2A, szTrack3A, dwRetry);
        }

        #endregion

        #region Concave/Indent Printing

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_PrintConcaveDigitsA")]
        private static extern uint SOY_PR_PrintConcaveDigitsA_Win(
            [MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA,
            uint dwReserved,
            uint dwDistance,
            uint dwSpace,
            [MarshalAs(UnmanagedType.LPStr)] string szDigitsA);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_PrintConcaveDigitsA")]
        private static extern uint SOY_PR_PrintConcaveDigitsA_Linux(
            [MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA,
            uint dwReserved,
            uint dwDistance,
            uint dwSpace,
            [MarshalAs(UnmanagedType.LPStr)] string szDigitsA);

        public static uint SOY_PR_PrintConcaveDigitsA(
            [MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA,
            uint dwReserved,
            uint dwDistance,
            uint dwSpace,
            [MarshalAs(UnmanagedType.LPStr)] string szDigitsA)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_PrintConcaveDigitsA_Win(szPrinterNameA, dwReserved, dwDistance, dwSpace, szDigitsA)
                : SOY_PR_PrintConcaveDigitsA_Linux(szPrinterNameA, dwReserved, dwDistance, dwSpace, szDigitsA);
        }

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_GetIndentConfigA")]
        private static extern uint SOY_PR_GetIndentConfigA_Win([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, uint dwConfigType, IntPtr lpConfigStruct);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_GetIndentConfigA")]
        private static extern uint SOY_PR_GetIndentConfigA_Linux([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, uint dwConfigType, IntPtr lpConfigStruct);

        public static uint SOY_PR_GetIndentConfigA([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, uint dwConfigType, IntPtr lpConfigStruct)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_GetIndentConfigA_Win(szPrinterNameA, dwConfigType, lpConfigStruct)
                : SOY_PR_GetIndentConfigA_Linux(szPrinterNameA, dwConfigType, lpConfigStruct);
        }

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_SetIndentConfigA")]
        private static extern uint SOY_PR_SetIndentConfigA_Win([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, uint dwConfigType, IntPtr lpConfigStruct);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_SetIndentConfigA")]
        private static extern uint SOY_PR_SetIndentConfigA_Linux([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, uint dwConfigType, IntPtr lpConfigStruct);

        public static uint SOY_PR_SetIndentConfigA([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, uint dwConfigType, IntPtr lpConfigStruct)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_SetIndentConfigA_Win(szPrinterNameA, dwConfigType, lpConfigStruct)
                : SOY_PR_SetIndentConfigA_Linux(szPrinterNameA, dwConfigType, lpConfigStruct);
        }

        #endregion

        #region Security

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_SetSecurityModeA")]
        private static extern uint SOY_PR_SetSecurityModeA_Win(
            [MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA,
            [MarshalAs(UnmanagedType.LPStr)] string szCurrentPasswdA,
            int nSecurityMode);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_SetSecurityModeA")]
        private static extern uint SOY_PR_SetSecurityModeA_Linux(
            [MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA,
            [MarshalAs(UnmanagedType.LPStr)] string szCurrentPasswdA,
            int nSecurityMode);

        public static uint SOY_PR_SetSecurityModeA(
            [MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA,
            [MarshalAs(UnmanagedType.LPStr)] string szCurrentPasswdA,
            int nSecurityMode)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_SetSecurityModeA_Win(szPrinterNameA, szCurrentPasswdA, nSecurityMode)
                : SOY_PR_SetSecurityModeA_Linux(szPrinterNameA, szCurrentPasswdA, nSecurityMode);
        }

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_SetSecurityPasswordA")]
        private static extern uint SOY_PR_SetSecurityPasswordA_Win(
            [MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA,
            [MarshalAs(UnmanagedType.LPStr)] string szCurrentPasswdA,
            [MarshalAs(UnmanagedType.LPStr)] string szNewPasswdA);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_SetSecurityPasswordA")]
        private static extern uint SOY_PR_SetSecurityPasswordA_Linux(
            [MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA,
            [MarshalAs(UnmanagedType.LPStr)] string szCurrentPasswdA,
            [MarshalAs(UnmanagedType.LPStr)] string szNewPasswdA);

        public static uint SOY_PR_SetSecurityPasswordA(
            [MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA,
            [MarshalAs(UnmanagedType.LPStr)] string szCurrentPasswdA,
            [MarshalAs(UnmanagedType.LPStr)] string szNewPasswdA)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_SetSecurityPasswordA_Win(szPrinterNameA, szCurrentPasswdA, szNewPasswdA)
                : SOY_PR_SetSecurityPasswordA_Linux(szPrinterNameA, szCurrentPasswdA, szNewPasswdA);
        }

        #endregion

        #region Firmware

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_GetBinFirmwareVersionA")]
        private static extern uint SOY_PR_GetBinFirmwareVersionA_Win([MarshalAs(UnmanagedType.LPStr)] string szBinUrlA, [Out] byte[] szBinVerOutA);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_GetBinFirmwareVersionA")]
        private static extern uint SOY_PR_GetBinFirmwareVersionA_Linux([MarshalAs(UnmanagedType.LPStr)] string szBinUrlA, [Out] byte[] szBinVerOutA);

        public static uint SOY_PR_GetBinFirmwareVersionA([MarshalAs(UnmanagedType.LPStr)] string szBinUrlA, [Out] byte[] szBinVerOutA)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_GetBinFirmwareVersionA_Win(szBinUrlA, szBinVerOutA)
                : SOY_PR_GetBinFirmwareVersionA_Linux(szBinUrlA, szBinVerOutA);
        }

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_UpdateFirmwareA")]
        private static extern uint SOY_PR_UpdateFirmwareA_Win([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, [MarshalAs(UnmanagedType.LPStr)] string szBinUrlA);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_UpdateFirmwareA")]
        private static extern uint SOY_PR_UpdateFirmwareA_Linux([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, [MarshalAs(UnmanagedType.LPStr)] string szBinUrlA);

        public static uint SOY_PR_UpdateFirmwareA([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA, [MarshalAs(UnmanagedType.LPStr)] string szBinUrlA)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_UpdateFirmwareA_Win(szPrinterNameA, szBinUrlA)
                : SOY_PR_UpdateFirmwareA_Linux(szPrinterNameA, szBinUrlA);
        }

        #endregion

        #region Job Management

        [DllImport("SeaorySDK.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SOY_PR_DeleteAllJobsA")]
        private static extern uint SOY_PR_DeleteAllJobsA_Win([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA);

        [DllImport("libSeaorySDK.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SOY_PR_DeleteAllJobsA")]
        private static extern uint SOY_PR_DeleteAllJobsA_Linux([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA);

        public static uint SOY_PR_DeleteAllJobsA([MarshalAs(UnmanagedType.LPStr)] string szPrinterNameA)
        {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? SOY_PR_DeleteAllJobsA_Win(szPrinterNameA)
                : SOY_PR_DeleteAllJobsA_Linux(szPrinterNameA);
        }

        #endregion
    }
}
