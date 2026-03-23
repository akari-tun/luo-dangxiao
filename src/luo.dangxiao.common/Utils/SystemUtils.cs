using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Management;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows;

namespace luo.dangxiao.common.Utils
{
    public class SystemUtils
    {
        [DllImport("user32.dll")]
        static extern int EnumDisplaySettings(string deviceName, int modelNum, ref DEVMODE devModel);

        [DllImport("user32.dll")]
        public static extern int ChangeDisplaySettings(ref DEVMODE devMode, int flags);

        [DllImport("user32.dll")]
        static extern IntPtr SendMessageW(IntPtr hWnd, int Msg, IntPtr wParam, IntPtr lParam);

        [DllImport("User32.dll")]
        private static extern bool ShowWindowAsync(IntPtr hWnd, int cmdShow);

        [DllImport("user32.dll")]
        public static extern int ShowWindow(IntPtr hwnd, int nCmdShow);

        [DllImport("User32.dll")]
        private static extern bool SetForegroundWindow(IntPtr hWnd);

        [DllImport("user32.dll")]
        static extern IntPtr GetForegroundWindows();

        [DllImport("gdi32.dll", EntryPoint = "GetDeviceCaps", SetLastError = true)]
        static extern int GetDeviceCaps(IntPtr hdc, int nIndex);

        [DllImport("user32.dll")]
        static extern bool GetMonitorInfo(IntPtr hMonitor, MONITORINFO lpmi);

        [DllImport("user32.dll")]
        static extern IntPtr MonitorFromWindow(IntPtr handle, int flags);

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        public struct DEVMODE
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
            public string dmDeviceName;

            public short dmSpecVersion;
            public short dmDriverVersion;
            public short dmSize;
            public short dmDriverExtra;
            public int dmFields;
            public int dmPositionX;
            public int dmPositionY;
            public int dmDisplayOrientation;
            public int dmDisplayFixedOutput;
            public short dmColor;
            public short dmDuplex;
            public short dmYResolution;
            public short dmTTOption;
            public short dmCollate;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
            public string dmFormName;
            public short dmLogPixels;
            public short dmBitsPerPel;
            public int dmPelsWidth;
            public int dmPelsHeight;
            public int dmDisplayFlags;
            public int dmDisplayFrequency;
            public int dmICMMethod;
            public int dmICMIntent;
            public int dmMediaType;
            public int dmDitherType;
            public int dmReserved1;
            public int dmReserved2;
            public int dmPanningWidth;
            public int dmPanningHeight;
        };

        // 控制改变屏幕分辨率的常量
        public const int ENUM_CURRENT_SETTINGS = -1;
        public const int CDS_UPDATEREGISTRY = 0x01;
        public const int CDS_TEST = 0x02;
        public const int DISP_CHANGE_SUCCESSFUL = 0;
        public const int DISP_CHANGE_RESTART = 1;
        public const int DISP_CHANGE_FAILED = -1;

        // 控制改变方向的常量定义
        public const int DMDO_DEFAULT = 0;
        public const int DMDO_90 = 1;
        public const int DMDO_180 = 2;
        public const int DMDO_270 = 3;

        [StructLayout(LayoutKind.Sequential)]
        public struct POINT
        {
            public int X;
            public int Y;

            public POINT(int x, int y)
            {
                this.X = x;
                this.Y = y;
            }
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct MINMAXINFO
        {
            public POINT ptReserved;
            public POINT ptMaxSize;
            public POINT ptMaxPosition;
            public POINT ptMinTrackSize;
            public POINT ptMaxTrackSize;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct RECT
        {
            public int Left, Top, Right, Bottom;
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
        public class MONITORINFO
        {
            public int cbSize = Marshal.SizeOf(typeof(MONITORINFO));
            public RECT rcMonitor = new RECT();
            public RECT rcWork = new RECT();
            public int dwFlags = 0;
        }

        enum DeviceCap
        {
            VERTRES = 10,
            PHYSICALWIDTH = 110,
            SCALINGFACTORX = 114,
            DESKTOPVERTRES = 117,
        }

        public class Pixels
        {
            public int Width { get; set; }
            public int Height { get; set; }

            public DEVMODE DevModel { get; set; }

            public static bool operator ==(Pixels l, Pixels r) => l.Width == r.Width && l.Height == r.Height;
            public static bool operator !=(Pixels l, Pixels r) => l.Width != r.Width || l.Height != r.Height;
            public static bool operator >(Pixels l, Pixels r)
            {
                if (l.Width > r.Width && l.Height > r.Height) return true;
                if (l.Width == r.Width && l.Height > r.Height) return true;
                if (l.Width > r.Width && l.Height == r.Height) return true;
                if (l.Width > r.Width) return true;

                return false;
            }
            public static bool operator <(Pixels l, Pixels r)
            {
                if (l.Width < r.Width && l.Height < r.Height) return true;
                if (l.Width == r.Width && l.Height < r.Height) return true;
                if (l.Width < r.Width && l.Height == r.Height) return true;
                if (l.Width < r.Width) return true;

                return false;
            }

            public override bool Equals(object obj) => this == (Pixels)obj;

            public override int GetHashCode() => HashCode.Combine(Width, Height);

            public override string ToString() => $"{Width}*{Height}";
        }

        public class Frequency
        {
            public int Hz { get; set; }

            public DEVMODE DevModel { get; set; }

            public static bool operator ==(Frequency l, Frequency r) => l.Hz == r.Hz;
            public static bool operator !=(Frequency l, Frequency r) => l.Hz != r.Hz;
            public static bool operator >(Frequency l, Frequency r) => l.Hz > r.Hz;
            public static bool operator <(Frequency l, Frequency r) => l.Hz < r.Hz;

            public override bool Equals(object obj) => this == (Frequency)obj;

            public override int GetHashCode() => Hz;

            public override string ToString() => $"{Hz}Hz";
        }

        private static SystemUtils instance = null;
        private readonly string cimv2 = @"\\.\root\cimv2";
        private readonly string wmi = @"\\.\root\wmi";
        private double _screenScalingFactor = 0;


        public static SystemUtils Instance
        {
            get
            {
                if (instance == null)
                {
                    instance = new SystemUtils();
                }

                return instance;
            }
        }

        public string GetComputerSystemModel()
        {
            var result = ExecuteWMIQuery("SELECT * FROM Win32_ComputerSystem", "Model");
            return (string)result[0];
        }

        public string GetCPUName()
        {
            var result = ExecuteWMIQuery("SELECT * FROM Win32_Processor", "Name");
            var name = (string)result[0];

            var split = name.Split('/');

            if (split.Length > 0)
            {
                return split[0];
            }

            return name;
        }

        public string GetDisplayAdapterName()
        {
            var result = ExecuteWMIQuery("SELECT * FROM Win32_VideoController", "Name");
            return (string)result[0];
        }

        public UInt64 GetMemoryCapacity()
        {
            UInt64 capacity = 0;

            var result = ExecuteWMIQuery("SELECT * FROM Win32_PhysicalMemory", "Capacity");
            foreach (var v in result)
            {
                capacity += (UInt64)v;
            }

            return capacity;
        }

        public UInt64 GetDiskSize()
        {
            UInt64 size = 0;

            //var result = ExecuteWMIQuery("SELECT * FROM Win32_DiskDrive", "Size");
            List<Tuple<object,object>> result = new List<Tuple<object, object>>();

            ManagementScope scope = new ManagementScope(cimv2);
            if (scope != null)
            {
                scope.Connect();
                if (scope.IsConnected)
                {
                    ObjectQuery query = new ObjectQuery("SELECT * FROM Win32_DiskDrive");
                    if (query != null)
                    {
                        ManagementObjectSearcher searcher = new ManagementObjectSearcher(scope, query);
                        if (searcher != null)
                        {
                            foreach (var m in searcher.Get())
                            {
                                result.Add(new Tuple<object, object>(m["Index"], m["Size"]));
                            }
                        }
                    }
                }
            }

            if (result.Count > 0)
            {
                if ((UInt32)result[0].Item1 == 0)
                {
                    size = (UInt64)result[0].Item2;

                }
            }

            return size;
        }

        public UInt32 GetBatteryFullChargeCapacity()
        {
            UInt32 capacity = 0;

            ManagementObjectSearcher searcher = new ManagementObjectSearcher(wmi, "SELECT * FROM BatteryFullChargedCapacity");
            foreach (var queryObj in searcher.Get())
            {
                if (queryObj["FullChargedCapacity"] != null)
                {
                    capacity = (UInt32)queryObj["FullChargedCapacity"];
                }
            }

            return capacity;
        }

        public string GetWirelessNetworkAdapter()
        {
            var result = ExecuteWMIQuery("SELECT * FROM Win32_NetworkAdapter", "Name");
            return (string)result.Find(x => { string s = (string)x; return (s.ToLower().Contains("wireless") || s.Contains("802.11") || s.ToLower().Contains("wlan")) && !s.ToLower().Contains("virtual"); });
        }

        public string GetCurrentDisplayName()
        {
            var result = ExecuteWMIQuery("SELECT * FROM Win32_DesktopMonitor ", "Name");
            return (string)result[0];
        }

        int _brightness;

        public int GetBrightness()
        {
            int capacity = 0;
            ManagementObjectSearcher searcher = new ManagementObjectSearcher(wmi, "SELECT * FROM WmiMonitorBrightness");
            foreach (var queryObj in searcher.Get())
            {
                if (queryObj["CurrentBrightness"] != null)
                {
                    capacity = (byte)queryObj["CurrentBrightness"];
                    break;
                }
            }

            _brightness = Convert.ToInt32(capacity);
            return _brightness;
        }

        public void SetBrightness(int brightness)
        {
            if (_brightness != brightness)
            {
                ManagementObjectSearcher searcher = new ManagementObjectSearcher(wmi, "SELECT * FROM WmiMonitorBrightnessMethods");
                foreach (ManagementObject queryObj in searcher.Get())
                {
                    var args = new object[] { 1, Convert.ToByte(brightness) };
                    queryObj.InvokeMethod("WmiSetBrightness", args);
                    break;
                }
            }
        }

        private List<object> ExecuteWMIQuery(string wmiquery, string field)
        {
            List<object> result = new List<object>();

            ManagementScope scope = new ManagementScope(cimv2);
            if (scope != null)
            {
                scope.Connect();
                if (scope.IsConnected)
                {
                    ObjectQuery query = new ObjectQuery(wmiquery);
                    if (query != null)
                    {
                        ManagementObjectSearcher searcher = new ManagementObjectSearcher(scope, query);
                        if (searcher != null)
                        {
                            foreach (var m in searcher.Get())
                            {
                                result.Add(m[field]);
                            }
                        }
                    }
                }
            }

            return result;
        }

        ManagementEventWatcher watcher = null;

        public void WmiWatcherStart(Action<byte> callback)
        {
            try
            {
                ManagementScope scope = new ManagementScope(wmi);
                WqlEventQuery query = new WqlEventQuery(@"SELECT * FROM IP3_WMIEvent");
                watcher = new ManagementEventWatcher(scope, query);
                watcher.EventArrived += (s, e) =>
                {
                    ManagementBaseObject ee = e.NewEvent;
                    Byte[] bytes = (byte[])ee.Properties["EventDetail"].Value;

                    if (bytes != null && bytes.Length >= 2 && bytes[1] == 0x38)
                    {
                        callback?.Invoke(bytes[1]);
                    }
                    else if (bytes[0] == 1 && bytes[1] == 0x11)
                    {
                        callback?.Invoke(bytes[1]);
                    }
                    else if (bytes[0] == 1 && bytes[1] == 0x12)
                    {
                        callback?.Invoke(bytes[1]);
                    }
                    else if (bytes[0] == 1 && bytes[1] == 0x13)
                    {
                        callback?.Invoke(bytes[1]);
                    }
                };

                watcher.Start();
            }
            catch (Exception ex)
            {
                if (watcher != null)
                {
                    watcher.Dispose();
                    watcher = null;
                }
            }
        }

        public void WmiWacherStop()
        {
            if (watcher != null)
            {
                watcher.Stop();
                watcher.Dispose();
                watcher = null;
            }
        }

        public (List<Pixels>, Dictionary<Pixels,List<Frequency>>) GetScreenPixels()
        {
            List<Pixels> pixels = new List<Pixels>();
            Dictionary<Pixels, List<Frequency>> freqs = new Dictionary<Pixels, List<Frequency>>();

            int nums = -1, ret = 0;
            DEVMODE devModel = default;
            devModel.dmSize = (short)Marshal.SizeOf(devModel);

            do
            {
                ret = EnumDisplaySettings(null, nums, ref devModel);
                Pixels pix = new Pixels() { Width = devModel.dmPelsWidth, Height = devModel.dmPelsHeight, DevModel = devModel };

                if (!pixels.Contains(pix))
                {
                    pixels.Add(pix);
                    freqs.Add(pix, new List<Frequency>());
                    freqs[pix].Add(new Frequency() { Hz = devModel.dmDisplayFrequency, DevModel = devModel });
                }
                else
                {
                    if (freqs.ContainsKey(pix))
                    {
                        freqs[pix].Add(new Frequency() { Hz = devModel.dmDisplayFrequency, DevModel = devModel });
                    }
                }

                nums++;
            } while (ret != 0);

            pixels.Sort(delegate (Pixels x, Pixels y)
            {
                if (x == y) return 0;
                if (x > y) return -1;
                if (x < y) return 1;

                return 0;
            });

            return (pixels, freqs);
        }

        public void ChangeResolution(DEVMODE devModel)
        {
            // 改变屏幕分辨率
            int iRet = ChangeDisplaySettings(ref devModel, CDS_TEST);

            if (iRet != DISP_CHANGE_FAILED)
            {
                iRet = ChangeDisplaySettings(ref devModel, CDS_UPDATEREGISTRY);

                switch (iRet)
                {
                    // 成功改变
                    case DISP_CHANGE_SUCCESSFUL:
                        {
                            break;
                        }
                    case DISP_CHANGE_RESTART:
                        {
                            Console.WriteLine("你需要重新启动电脑设置才能生效");
                            break;
                        }
                    default:
                        {
                            Console.WriteLine("改变屏幕分辨率失败");
                            break;
                        }
                }
            }
        }

        public double GetScreenScalingFactor()
        {
            var g = Graphics.FromHwnd(IntPtr.Zero);
            IntPtr desktop = g.GetHdc();
            var physicalScreenHeight = GetDeviceCaps(desktop, (int)DeviceCap.DESKTOPVERTRES);
            _screenScalingFactor = physicalScreenHeight / SystemParameters.PrimaryScreenHeight;

            return _screenScalingFactor;
        }

        public Process RunningInstance()
        {
            Process current = Process.GetCurrentProcess();
            Process[] processes = Process.GetProcessesByName(current.ProcessName);
            //遍历与当前进程名称相同的进程列表  
            foreach (Process process in processes)
            {
                //如果实例已经存在则忽略当前进程  
                if (process.Id != current.Id)
                {
                    //返回已经存在的进程
                    return process;
                }
            }
            return null;
        }

        public void ShowProcess(Process process)
        {
            SendMessageW(process.MainWindowHandle, WinApi.WM_USER + 1, IntPtr.Zero, IntPtr.Zero);
        }

        static string _model = string.Empty;
        public static string GetModel()
        {
            try
            {
                if (string.IsNullOrEmpty(_model))
                {
                    // 获取当前应用程序的目录
                    string currentDirectory = AppDomain.CurrentDomain.BaseDirectory;
                    //string dmidecodePath = System.IO.Path.Combine(currentDirectory, "dmidecode.exe");
                    string dmidecodePath = "dmidecode.exe";

                    // 准备启动 cmd 进程
                    Process process = new Process();
                    process.StartInfo.FileName = "cmd.exe";
                    process.StartInfo.WorkingDirectory = currentDirectory;
                    process.StartInfo.UseShellExecute = false;
                    process.StartInfo.CreateNoWindow = true; // 设置为隐藏窗口
                    process.StartInfo.RedirectStandardOutput = true;
                    process.StartInfo.RedirectStandardInput = true;
                    // 启动 cmd 进程
                    process.Start();
                    process.StandardInput.WriteLine($"{dmidecodePath} -t 11 | find \"String 10:\"");
                    process.StandardInput.WriteLine("exit"); // 退出命令行窗口
                    process.StandardInput.Flush();
                    process.StandardInput.Close();
                    string output = process.StandardOutput.ReadToEnd();
                    process.WaitForExit();
                    process.Close();

                    if (output.Contains("String 10: ARP31B"))
                    {
                        _model = "ARP31B";
                    }
                    else
                    {
                        _model = "ARP31";
                    }
                }

                return _model;
            }
            catch
            {
                // 捕获并处理异常
                //MessageBox.Show($"访问 dmidecode.exe 失败：{ex.Message}", "错误", MessageBoxButton.OK, MessageBoxImage.Error);
                return "ARP31";
            }
        }

        public static void WmGetMinMaxInfo(IntPtr hwnd, IntPtr lParam)
        {
            MINMAXINFO mmi = Marshal.PtrToStructure<MINMAXINFO>(lParam);

            // 获取屏幕尺寸
            int MONITOR_DEFAULTTONEAREST = 0x00000002;
            IntPtr monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);

            if (monitor != IntPtr.Zero)
            {
                MONITORINFO monitorInfo = new MONITORINFO();
                GetMonitorInfo(monitor, monitorInfo);
                RECT rcWorkArea = monitorInfo.rcWork;
                RECT rcMonitorArea = monitorInfo.rcMonitor;

                mmi.ptMaxPosition.X = Math.Abs(rcWorkArea.Left - rcMonitorArea.Left);
                mmi.ptMaxPosition.Y = Math.Abs(rcWorkArea.Top - rcMonitorArea.Top);
                mmi.ptMaxSize.X = Math.Abs(rcWorkArea.Right - rcWorkArea.Left);
                mmi.ptMaxSize.Y = Math.Abs(rcWorkArea.Bottom - rcWorkArea.Top);
            }

            Marshal.StructureToPtr(mmi, lParam, true);
        }
    }
}
