/* ***********************************************
* Author:          谭骏 (32965926@qq.com)
* Create Time:     2013-11-28
* CopyRight:       Copyright (C) 2008-2013 深圳市宇川智能系统有限公司 All Rights Reserved
* Description:     WindowsApi定义
* ***********************************************/

using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace luo.dangxiao.common.Utils
{
    /// <summary>
    /// WinApi定义
    /// </summary>
    public static class WinApi
    {
        #region 常量
        public const int WM_HOTKEY = 0x0312;
        public const int WM_KEYUP = 0x101;
        public const int WM_CHAR = 0x102;
        public const int WM_SYSKEYDOWN = 0x104;
        public const int WM_SYSKEYUP = 0x105;
        public const int WM_KEYDOWN = 0x100;
        public const int WM_COMMAND = 0x111;
        public const int WM_SETTEXT = 0x0C;
        public const int WM_GETTEXT = 0x0D;
        public const int WM_GETTEXTLENGTH = 0x0E;
        public const int GW_ENABLEDPOPUP = 6;
        public const int BM_CLICK = 0xF5;
        public const int WM_SYSCOMMAND = 0x0112;

        public const int GW_HWNDFIRST = 0;
        public const int GW_HWNDLAST = 1;
        public const int GW_HWNDNEXT = 2;
        public const int GW_HWNDPREV = 3;
        public const int GW_OWNER = 4;

        public const int SM_HIDE = 0;
        public const int SW_SHOW = 5;
        public const int WM_CLOSE = 0x0010;
        public const int SC_CLOSE = 0x0060;
        public const int WM_LBUTTONDOWN = 0x201;
        public const int WM_LBUTTONUP = 0x202;
        public const int VK_C = 67;

        public const int WM_MOUSEMOVE = 0x200;
        public const int MK_LBUTTON = 0x1;
        public const int MK_MBUTTON = 0x10;

        public const int WM_COPYDATA = 0x004A;
        public const int WM_USER = 0x0400;

        // 定义窗口插入位置常量
        public static IntPtr HWND_TOPMOST = new IntPtr(-1);
        public const int GWL_HWDPARENT = -8;
        public const uint SWP_NOSIZE = 0x0001;
        public const uint SWP_NOMOVE = 0x0002;
        public const uint SWP_NOACTIVATE = 0x0010;
        public const uint SWP_SHOWWINDOW = 0x40;

        public const int WM_GETMINMAXINFO = 0x0024;

        #endregion

        /// <summary>
        /// 发送消息给窗体  
        /// </summary>
        /// <param name="hWnd">窗口句柄</param>
        /// <param name="Msg">消息</param>
        /// <param name="wParam">W参数</param>
        /// <param name="lParam">L参数</param>
        /// <returns></returns>
        [DllImport("User32.dll", EntryPoint = "SendMessage")]
        public static extern Int32 SendMessage(IntPtr hWnd, int Msg, int wParam, StringBuilder lParam);

        /// <summary>
        /// 发送消息给窗体  
        /// </summary>
        /// <param name="hWnd"></param>
        /// <param name="Msg"></param>
        /// <param name="wParam"></param>
        /// <param name="lParam"></param>
        /// <returns></returns>
        [DllImport("User32.dll", EntryPoint = "SendMessage")]
        public static extern int SendMessage(IntPtr hWnd, int Msg, IntPtr wParam, string lParam);

        /// <summary>
        /// 发送消息给窗体  
        /// </summary>
        /// <param name="hWnd"></param>
        /// <param name="Msg"></param>
        /// <param name="wParam"></param>
        /// <param name="lParam"></param>
        /// <returns></returns>
        [DllImport("User32.dll", EntryPoint = "SendMessage")]
        public static extern int SendMessage(IntPtr hWnd, int Msg, int wParam, string lParam);

        /// <summary>
        /// 发送消息给窗体  
        /// </summary>
        /// <param name="hWnd"></param>
        /// <param name="Msg"></param>
        /// <param name="wParam"></param>
        /// <param name="lParam"></param>
        /// <returns></returns>
        [DllImport("User32.dll", EntryPoint = "SendMessage")]
        public static extern int SendMessage(IntPtr hWnd, int Msg, int wParam, int lParam);

        /// <summary>
        /// 将一条消息放入到消息队列中
        /// </summary>
        /// <param name="hWnd"></param>
        /// <param name="Msg"></param>
        /// <param name="wParam"></param>
        /// <param name="lParam"></param>
        /// <returns></returns>
        [DllImport("User32.dll")]
        public static extern Int32 PostMessage(IntPtr hWnd, int Msg, int wParam, int lParam);

        /// <summary>
        /// 取得前台窗口句柄
        /// </summary>
        /// <returns></returns>
        [DllImport("User32.dll", EntryPoint = "GetForegroundWindow")]
        public static extern IntPtr GetForegroundWindow();

        /// <summary>
        /// 取得焦点句柄
        /// </summary>
        /// <returns></returns>
        [DllImport("User32.dll", EntryPoint = "GetFocus")]
        public static extern IntPtr GetFocus();

        /// <summary>
        /// 连接到另外的线程共享输入
        /// </summary>
        /// <param name="idAttach"></param>
        /// <param name="idAttachTo"></param>
        /// <param name="fAttach"></param>
        /// <returns></returns>
        [DllImport("User32.dll", EntryPoint = "AttachThreadInput")]
        public static extern IntPtr AttachThreadInput(IntPtr idAttach, IntPtr idAttachTo, int fAttach);

        /// <summary>
        /// 找出某个窗口的创建者（线程或进程）
        /// </summary>
        /// <param name="hWnd"></param>
        /// <param name="ProcessId"></param>
        /// <returns></returns>
        [DllImport("User32.dll", EntryPoint = "GetWindowThreadProcessId")]
        public static extern IntPtr GetWindowThreadProcessId(IntPtr hWnd, IntPtr ProcessId);
        [DllImport("kernel32.dll", EntryPoint = "GetLastError")]
        public static extern int GetLastError();

        /// <summary>
        /// 获取当前线程的ID
        /// </summary>
        /// <returns></returns>
        [DllImport("kernel32.dll", EntryPoint = "GetCurrentThreadId")]
        public static extern IntPtr GetCurrentThreadId();

        [DllImport("user32.dll", EntryPoint = "GetDesktopWindow", SetLastError = false)]
        public static extern IntPtr GetDesktopWindow();

        [DllImport("User32.dll", EntryPoint = "FindWindowEx")]
        public static extern IntPtr FindWindowEx(IntPtr hwndParent, IntPtr hwndChildAfter, string lpClassName, string lpWindowName);

        // 导入 SetWindowPos 函数
        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool SetWindowPos(IntPtr hWnd, IntPtr hWndInsertAfter, int X, int Y, int cx, int cy, uint uFlags);

        // 导入 SetWindowPos 函数
        [DllImport("user32.dll")]
        public static extern bool SetWindowLong(IntPtr hWnd, int nIndex, IntPtr dwNewLong);
    }
}
