using luo.dangxiao.cardreader.Yc.Structs;
using System.IO;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text;

namespace luo.dangxiao.cardreader.Yc;

public class LicenseManager
{
    byte[] InitialTr ={
                 58,50,42,34,26,18,10,02,
                 60,52,44,36,28,20,12,04,
                 62,54,46,38,30,22,14,06,
                 64,56,48,40,32,24,16,0x08,
                 57,49,41,33,25,17,0x09,01,
                 59,51,43,35,27,19,11,03,
                 61,53,45,37,29,21,13,05,
                 63,55,47,39,31,23,15,07 };

    byte[] FinalTr ={
                 40,0x08,48,16,56,24,64,32,
                 39,07,47,15,55,23,63,31,
                 38,06,46,14,54,22,62,30,
                 37,05,45,13,53,21,61,29,
                 36,04,44,12,52,20,60,28,
                 35,03,43,11,51,19,59,27,
                 34,02,42,10,50,18,58,26,
                 33,01,41,0x09,49,17,57,25 };

    byte[] Swap = {
                 33,34,35,36,37,38,39,40,
                 41,42,43,44,45,46,47,48,
                 49,50,51,52,53,54,55,56,
                 57,58,59,60,61,62,63,64,
                 01,02,03,04,05,06,07,0x08,
                 0x09,10,11,12,13,14,15,16,
                 17,18,19,20,21,22,23,24,
                 25,26,27,28,29,30,31,32 };

    byte[] KeyTr1 = {
                 57,49,41,33,25,17,0x09,
                 01,58,50,42,34,26,18,
                 10,02,59,51,43,35,27,
                 19,11,03,60,52,44,36,

                 63,55,47,39,31,23,15,
                 07,62,54,46,38,30,22,
                 14,06,61,53,45,37,29,
                 21,13,05,28,20,12,04,

                 00,00,00,00,00,00,00,00 };

    byte[] KeyTr2 = {
                 14,17,11,24,01,05,
                 03,28,15,06,21,10,
                 23,19,12,04,26,0x08,
                 16,07,27,20,13,02,
                 41,52,31,37,47,55,
                 30,40,51,45,33,48,
                 44,49,39,56,34,53,
                 46,42,50,36,29,32,

                 00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00 };

    byte[] etr = {
                 32,01,02,03,04,05,
                 04,05,06,07,0x08,0x09,
                 0x08,0x09,10,11,12,13,
                 12,13,14,15,16,17,
                 16,17,18,19,20,21,
                 20,21,22,23,24,25,
                 24,25,26,27,28,29,
                 28,29,30,31,32,01,

                 00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00 };

    byte[] ptr =
    {
        16,07,20,21,
        29,12,28,17,
        01,15,23,26,
        05,18,31,10,
        02,0x08,24,14,
        32,27,03,0x09,
        19,13,30,06,
        22,11,04,25,

        00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
        00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00
    };

    byte[][] s =
    {
        new byte[] {
         14,04,13,01,02,15,11,0x08,03,10,06,12,05,0x09,00,07,
         00,15,07,04,14,02,13,01,10,06,12,11,0x09,05,03,0x08,
         01,01,14,0x08,13,06,02,11,15,12,0x09,07,03,10,05,00,
         15,12,0x08,02,04,0x09,01,07,05,11,03,14,10,00,06,13
        },
        new byte[] {
         15,01,0x08,14,06,11,03,04,0x09,07,02,13,12,00,05,10,
         03,13,04,07,15,02,0x08,15,12,00,01,10,06,0x09,11,05,
         00,14,17,11,10,04,13,01,05,0x08,12,06,0x09,03,02,15,
         13,0x08,10,01,03,15,04,02,11,06,07,12,00,05,14,0x09
        },
        new byte[] {
         10,00,0x09,14,06,03,15,05,01,13,12,07,11,04,02,0x08,
         13,07,00,0x09,03,04,06,10,02,0x08,05,14,12,11,15,01,
         13,06,04,0x09,0x08,15,03,00,11,01,02,12,05,10,14,07,
         01,10,13,00,06,0x09,0x08,07,04,15,14,03,11,05,02,12
        },
        new byte[] {
         07,13,14,03,00,06,0x09,10,01,02,0x08,05,11,12,04,15,
         13,0x08,11,05,06,15,00,03,04,07,02,12,01,10,14,0x09,
         10,06,0x09,00,12,11,07,13,15,01,03,14,05,02,0x08,04,
         03,15,00,06,10,10,13,0x08,0x09,04,05,11,12,07,02,14
        },
        new byte[] {
         02,12,04,01,07,10,11,06,0x08,05,03,15,13,00,14,0x09,
         14,11,02,12,04,07,13,01,05,00,15,10,03,0x09,0x08,06,
         04,02,01,11,10,13,07,0x08,15,0x09,12,05,06,03,00,14,
         11,0x08,12,07,01,14,02,13,06,15,00,0x09,10,04,05,03
        },
        new byte[] {
         12,01,10,15,0x09,02,06,0x08,00,13,03,04,14,07,05,11,
         10,15,04,02,07,12,0x09,05,06,01,13,14,00,11,03,0x08,
         0x09,14,15,05,02,0x08,12,03,07,00,04,10,01,13,11,06,
         04,03,02,12,0x09,05,15,10,11,14,01,07,06,00,0x08,13
        },
        new byte[] {
         04,11,02,14,15,00,0x08,13,03,12,0x09,07,05,10,06,01,
         13,00,11,07,04,0x09,01,10,14,03,05,12,02,15,0x08,06,
         01,04,11,13,12,03,07,14,10,15,06,0x08,00,05,0x09,02,
         06,11,13,0x08,01,04,10,07,0x09,05,00,15,14,02,03,12
        },
        new byte[] {
         13,02,0x08,04,06,15,11,01,10,0x09,03,14,05,00,12,07,
         01,15,13,0x08,10,03,07,04,12,05,06,11,00,14,0x09,02,
         07,11,04,01,0x09,12,14,02,00,06,10,13,15,03,05,0x08,
         02,01,14,07,04,10,0x08,13,15,12,0x09,00,03,05,06,11
        }
    };

    byte[] rots = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2 };
    byte[][] CKey = new byte[16][]
    {
        new byte[64], new byte[64], new byte[64], new byte[64],
        new byte[64], new byte[64], new byte[64], new byte[64],
        new byte[64], new byte[64], new byte[64], new byte[64],
        new byte[64], new byte[64], new byte[64], new byte[64]
    };

    private static LicenseManager? _instance;
    private static readonly object _lock = new();

    public SystemInfoNew SystemInfo { get; private set; }
    public bool HasAccessControlAuth { get; private set; }
    public bool HasPaymentAuth { get; private set; }
    public bool HasWaterBillingAuth { get; private set; }

    public static LicenseManager Instance
    {
        get
        {
            if (_instance == null)
            {
                lock (_lock)
                {
                    _instance ??= new LicenseManager();
                }
            }
            return _instance;
        }
    }

    private LicenseManager()
    {
        SystemInfo = new SystemInfoNew();
    }

    public void SetSystemInfo(SystemInfoNew info)
    {
        SystemInfo = info;
        HasAccessControlAuth = info.AccessControlSystem != 0;
        HasPaymentAuth = info.PaymentSystem != 0;
        HasWaterBillingAuth = info.WaterBillingSystem != 0;
    }

    public void ClearAuth()
    {
        HasAccessControlAuth = false;
        HasPaymentAuth = false;
        HasWaterBillingAuth = false;
    }

    /// <summary>
    /// Load licence from file
    /// </summary>
    /// <param name="filePath">Path to licencecard.dat file</param>
    /// <returns>Error code (0 = success)</returns>
    public int LoadLicence(string filePath)
    {
        try
        {
            if (!File.Exists(filePath))
                return -2; // File not found

            // Read 1024 bytes from file
            byte[] licenceData = File.ReadAllBytes(filePath);
            if (licenceData.Length != 1024)
                return -3; // Invalid file size

            // Verify CRC on first 1022 bytes
            ushort crc = CalculateCrc16(licenceData, 1022);
            ushort fileCrc = (ushort)(licenceData[1022] | (licenceData[1023] << 8));
            if (crc != fileCrc)
                return -4; // CRC mismatch

            IntPtr ptrLic = Marshal.AllocHGlobal(120);

            try
            {
                byte[] key12 = System.Text.Encoding.ASCII.GetBytes("20060110");

                for (int i = 0; i < 15; i++)
                {
                    // Decrypt system info at offset 0x308 (776)
                    // System info is 120 bytes, encrypted in 15 blocks of 8 bytes each using DES
                    byte[] block = new byte[8];
                    Buffer.BlockCopy(licenceData, i * 8 + 0x308, block, 0, 8);

                    byte[] _srcblock = ExpandT(block);
                    byte[] _key = ExpandT(key12);
                    byte[] _ct = UnDes(_srcblock, _key);
                    byte[] _result = PackingT(_ct);

                    // Copy decypted block to licence point
                    Marshal.Copy(_result, 0, ptrLic + i * 8, _result.Length);
                }

                // Parse system info from decrypted data
                var sysInfo = Marshal.PtrToStructure<SystemInfoNew>(ptrLic + 5);

                // Set system info and update auth flags
                SetSystemInfo(sysInfo);
            }
            finally
            {
                Marshal.FreeHGlobal(ptrLic);
            }

            return 0; // Success
        }
        catch (Exception ex)
        {
            System.Diagnostics.Debug.WriteLine($"LoadLicence error: {ex.Message}");
            return -1; // Read error
        }
    }

    /// <summary>
    /// Calculate CRC16 for licence verification using CCITT algorithm
    /// Matches C++ implementation: crc ^= data[i] << 8, shift left, polynomial 0x1021
    /// </summary>
    private ushort CalculateCrc16(byte[] data, int length)
    {
        ushort crc = 0xFFFF;
        for (int i = 0; i < length && i < data.Length; i++)
        {
            crc ^= (ushort)(data[i] << 8);
            for (int j = 0; j < 8; j++)
            {
                if ((crc & 0x8000) != 0)
                {
                    crc <<= 1;
                    crc ^= 0x1021;
                }
                else
                {
                    crc <<= 1;
                }
            }
        }
        return crc;
    }

    private byte[] UnDes(byte[] ciphertext, byte[] key)
    {
        int i, j;
        byte[] a = new byte[64], b = new byte[64], x = new byte[64];

        Array.Copy(ciphertext, a, 64);
        TransPose(ref a, InitialTr, 64);
        TransPose(ref key, KeyTr1, 56);
        SetKey(ref key);
        for (i = 0; i < 16; i++)
        {
            Array.Copy(a, b, 64);
            for (j = 0; j < 32; j++)
                a[j] = b[j + 32];

            f(15 - i, a, ref x);
            for (j = 0; j < 32; j++)
                if (b[j] + x[j] == 1) a[j + 32] = 1;
                else a[j + 32] = 0;
        }
        TransPose(ref a, Swap, 64);
        TransPose(ref a, FinalTr, 64);
        return a;
    }


    void TransPose(ref byte[] data, byte[] t, int n)
    {
        byte[] x = new byte[64];
        int i;

        Array.Copy(data, x, 64);

        for (i = 0; i < n; i++)
            data[i] = x[t[i] - 1];
    }

    void SetKey(ref byte[] key)
    {
        byte[] ikey = new byte[64];
        int i, j;

        Array.Copy(key, ikey, 64);

        for (i = 0; i < 16; i++)
        {
            for (j = 0; j < rots[i]; j++)
                Rotate(ref ikey);

            Array.Copy(ikey, CKey[i], 64);
            TransPose(ref CKey[i], KeyTr2, 48);
        }
    }

    void Rotate(ref byte[] key)
    {
        int i;
        byte[] x = new byte[64];

        for (i = 0; i < 56; i++)
            x[i] = key[i];

        for (i = 0; i < 55; i++)
            x[i] = x[i + 1];

        x[27] = key[1];
        x[55] = key[28];

        for (i = 0; i < 56; i++)
            key[i] = x[i];
    }

    void f(int i, byte[] a, ref byte[] x)
    {
        byte ch = 0;
        byte[] e = new byte[64], ikey = new byte[64], y = new byte[64];
        int r, k, j;

        Array.Copy(a, e, 64);
        TransPose(ref e, etr, 48);
        Array.Copy(CKey[i], ikey, 64);
        for (j = 0; j < 48; j++)
            if (e[j] + ikey[j] == 1) y[j] = 1;
            else y[j] = 0;

        for (k = 0; k < 8; k++)
        {
            r = 32 * y[6 * k + 5] + 16 * y[6 * k] + 8 * y[6 * k + 4] + 4 * y[6 * k + 3] + 2 * y[6 * k + 2] + y[6 * k + 1];

            ch = s[k][r];
            if ((ch & 8) != 0) x[4 * k] = 1;
            else x[4 * k] = 0;
            if ((ch & 4) != 0) x[4 * k + 1] = 1;
            else x[4 * k + 1] = 0;
            if ((ch & 2) != 0) x[4 * k + 2] = 1;
            else x[4 * k + 2] = 0;
            if ((ch & 1) != 0) x[4 * k + 3] = 1;
            else x[4 * k + 3] = 0;
        }
        TransPose(ref x, ptr, 32);
    }

    private bool Check_Crc(byte[] data)
    {
        ushort crc, ii;
        int i, j;
        ushort CRC_POLYNOM = 0x1021;

        crc = ushort.MaxValue;
        for (i = 0; i < data.Length - 2; i++)
        {
            crc ^= (ushort)(data[i] << 8);
            for (j = 0; j < 8; j++)
            {
                if ((crc & 0x8000) != 0)
                    crc = (ushort)((crc << 1) ^ CRC_POLYNOM);
                else
                    crc = (ushort)(crc << 1);
            }
        }

        ii = (ushort)((data[data.Length - 1] << 8) + data[data.Length - 2]);

        return ii == crc;
    }

    private byte[] ExpandT(byte[] s)
    {
        int i, j;
        byte ch, k;
        byte[] ret = new byte[s.Length * 8];

        for (i = 0; i < 8; i++)
        {
            ch = s[i];
            k = 0x80;
            for (j = 0; j < 8; j++)
            {
                if ((ch & (k >> j)) != 0) ret[i * 8 + j] = 1;
                else ret[i * 8 + j] = 0;
            }
        }

        return ret;
    }

    private byte[] PackingT(byte[] s)
    {
        int i, j;
        byte ch, k;
        byte[] ret = new byte[s.Length / 8];

        for (i = 0; i < 8; i++)
        {
            k = 0x80; ch = 0;
            for (j = 0; j < 8; j++)
            {
                if (s[i * 8 + j] != 0) ch = (byte)(ch | (k >> j));
                else ch = (byte)(ch & ~(k >> j));
            }
            ret[i] = ch;
        }

        return ret;
    }

    /// <summary>
    /// Parse system info from decrypted byte array
    /// </summary>
    private SystemInfoNew ParseSystemInfo(byte[] data)
    {
        var sysInfo = new SystemInfoNew();
        int pos = 0;

        // Skip 2 bytes (ch[2])
        pos += 2;

        // Parse system info (little-endian)
        // C++: mjxt = Access Control System
        sysInfo.AccessControlSystem = BitConverter.ToInt32(data, pos);
        pos += 4;
        
        // C++: mjxt_sec = Access Control Sector
        sysInfo.AccessControlSector = BitConverter.ToInt32(data, pos);
        pos += 4;
        
        // C++: mjxt_cardtype = Access Control Card Type
        sysInfo.AccessControlCardType = data[pos++];
        
        // C++: xfxt = Payment System
        sysInfo.PaymentSystem = BitConverter.ToInt32(data, pos);
        pos += 4;
        
        // C++: xfxt_sec = Payment Sector
        sysInfo.PaymentSector = BitConverter.ToInt32(data, pos);
        pos += 4;
        
        // C++: jsxt = Water Billing System
        sysInfo.WaterBillingSystem = BitConverter.ToInt32(data, pos);
        pos += 4;
        
        // C++: jsxt_sec = Water Billing Sector
        sysInfo.WaterBillingSector = BitConverter.ToInt32(data, pos);
        pos += 4;

        // System card numbers (5 bytes each)
        // C++: syscard_no_xf = Payment system card number
        Buffer.BlockCopy(data, pos, sysInfo.SystemCardNumberPayment, 0, 5);
        pos += 5;
        
        // C++: syscard_no_mj = Access control system card number
        Buffer.BlockCopy(data, pos, sysInfo.SystemCardNumberAccess, 0, 5);
        pos += 5;
        
        // C++: syscard_no_js = Water billing system card number
        Buffer.BlockCopy(data, pos, sysInfo.SystemCardNumberWater, 0, 5);
        pos += 5;

        // Passwords (8 bytes each)
        Buffer.BlockCopy(data, pos, sysInfo.SystemKeyB, 0, 8);
        pos += 8;
        
        Buffer.BlockCopy(data, pos, sysInfo.UserPassword, 0, 8);
        pos += 8;
        
        Buffer.BlockCopy(data, pos, sysInfo.OperatorPassword, 0, 8);
        pos += 8;
        
        Buffer.BlockCopy(data, pos, sysInfo.CommunicationPassword, 0, 8);
        pos += 8;

        // Reserved (6 bytes) - unuser[6]
        Buffer.BlockCopy(data, pos, sysInfo.Reserved2, 0, 6);
        pos += 6;

        return sysInfo;
    }
}

public static class KeyCalculator
{
    public static readonly byte[] DefaultKeyA1 = { 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 };
    public static readonly byte[] DefaultKeyA2 = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    public static readonly byte[] DefaultKeyB1 = { 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5 };
    public static readonly byte[] DefaultKeyB2 = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    
    public static readonly byte[] SystemCardKeyA12 = { 0xCC, 0x11, 0x22, 0x33, 0x44, 0x3C };
    public static readonly byte[] SystemCardKeyB12 = { 0xAC, 0x09, 0x87, 0x65, 0x43, 0x21 };
    
    public static readonly byte[] UserCard1KeyA = { 0xF1, 0xCF, 0xD3, 0xED, 0xF9, 0x58 };
    public static readonly byte[] LockCardKeyB = { 0xDD, 0xEA, 0xFC, 0xA0, 0xBC, 0xD1 };
    public static readonly byte[] LockCardKeyBNew = { 0x78, 0x28, 0x1F, 0x88, 0x5F, 0x78 };

    public static readonly byte[] AccessBytes1 = { 0xFF, 0x07, 0x80 };
    public static readonly byte[] AccessBytes2 = { 0x7F, 0x07, 0x88 };
    
    public static readonly byte[] ControlWord = { 0x7F, 0x07, 0x88, 0x69 };
    public static readonly byte[] ControlWord12 = { 0x7F, 0x07, 0x88, 0xDA };

    public static void CalculateKey12(byte[] cardSerial, byte[] password, byte[] outputKey)
    {
        if (cardSerial == null || cardSerial.Length < 4)
            throw new ArgumentException("Card serial must be at least 4 bytes", nameof(cardSerial));
        if (password == null || password.Length < 6)
            throw new ArgumentException("Password must be at least 6 bytes", nameof(password));
        if (outputKey == null || outputKey.Length < 16)
            throw new ArgumentException("Output key must be at least 16 bytes", nameof(outputKey));

        outputKey[0] = (byte)(password[0] + cardSerial[0]);
        outputKey[1] = (byte)(password[1] + cardSerial[1]);
        outputKey[2] = (byte)(password[2] + cardSerial[2]);
        outputKey[3] = (byte)(password[3] - cardSerial[3]);
        outputKey[4] = (byte)(password[4] ^ cardSerial[2]);
        outputKey[5] = (byte)(password[5] ^ cardSerial[1]);

        outputKey[6] = 0x7F;
        outputKey[7] = 0x07;
        outputKey[8] = 0x88;
        outputKey[9] = 0xDA;

        outputKey[10] = 0x78;
        outputKey[11] = 0x28;
        outputKey[12] = 0x1F;
        outputKey[13] = 0x88;
        outputKey[14] = 0x5F;
        outputKey[15] = 0x78;
    }

    public static void CalculateKey12New(byte[] cardSerial, byte[] userCode, byte[] outputKey)
    {
        if (cardSerial == null || cardSerial.Length < 4)
            throw new ArgumentException("Card serial must be at least 4 bytes", nameof(cardSerial));
        if (userCode == null || userCode.Length < 16)
            throw new ArgumentException("User code must be at least 16 bytes", nameof(userCode));
        if (outputKey == null || outputKey.Length < 16)
            throw new ArgumentException("Output key must be at least 16 bytes", nameof(outputKey));

        outputKey[0] = (byte)(userCode[2] + cardSerial[0] + 0x11);
        outputKey[1] = (byte)(userCode[1] - cardSerial[1] + 0x22);
        outputKey[2] = (byte)((userCode[0] | cardSerial[2]) + 0x33);
        outputKey[3] = (byte)((userCode[3] & cardSerial[3]) + 0x44);
        outputKey[4] = (byte)((userCode[4] ^ cardSerial[2]) + userCode[6] + 0x55);
        outputKey[5] = (byte)((userCode[5] ^ cardSerial[1]) + userCode[7] + 0x66);

        outputKey[6] = 0xFF;
        outputKey[7] = 0x07;
        outputKey[8] = 0x80;
        outputKey[9] = 0x69;

        outputKey[10] = 0x78;
        outputKey[11] = 0x28;
        outputKey[12] = 0x1F;
        outputKey[13] = 0x88;
        outputKey[14] = 0x5F;
        outputKey[15] = 0x78;
    }

    public static byte CalculateBcc(byte[] data, int length)
    {
        byte bcc = 0;
        for (int i = 0; i < length && i < data.Length; i++)
        {
            bcc += data[i];
        }
        return bcc;
    }

    /// <summary>
    /// DES-like transformation for password processing
    /// </summary>
    public static void DesTransform(byte[] input, byte[] output)
    {
        if (input == null || input.Length < 8)
            throw new ArgumentException("Input must be at least 8 bytes", nameof(input));
        if (output == null || output.Length < 8)
            throw new ArgumentException("Output must be at least 8 bytes", nameof(output));

        // Key from C++: key12[]="20060110"
        byte[] key = { 0x32, 0x30, 0x30, 0x36, 0x30, 0x31, 0x31, 0x30 }; // "20060110"
        
        // Simple XOR-based transformation (matching C++ logic)
        for (int i = 0; i < 8; i++)
        {
            output[i] = (byte)(input[i] ^ key[i]);
        }
    }

    public static bool VerifyBcc(byte[] data)
    {
        if (data == null || data.Length < 16) return false;
        
        byte bcc = 0;
        for (int i = 0; i < 15; i++)
        {
            bcc += data[i];
        }
        return bcc == data[15];
    }
}

public class UserCode
{
    public byte[] Data { get; } = new byte[48];
    
    public byte EncryptionMode
    {
        get => Data[8];
        set => Data[8] = value;
    }

    public UserCode()
    {
        Data.AsSpan().Fill(0);
        Data[8] = 0x01;
    }

    public UserCode(byte[] data)
    {
        if (data != null && data.Length >= 48)
        {
            Buffer.BlockCopy(data, 0, Data, 0, 48);
        }
    }

    public byte[] GetKeyA() => Data.AsSpan(0, 6).ToArray();
    public byte[] GetKeyB() => Data.AsSpan(10, 6).ToArray();

    public static UserCode BuildUserCode(int keyMode, string empStrId, string empName, string cardTypeName)
    {
        var userCode = new UserCode();
        using var ms = new MemoryStream(userCode.Data);
        using var bw = new BinaryWriter(ms);

        byte[] empIdBytes = Encoding.ASCII.GetBytes(empStrId.Length > 5 ? empStrId.Substring(0, 5) : empStrId);
        byte[] empNameBytes = Encoding.GetEncoding("GB2312").GetBytes(empName);
        byte[] typeNameBytes = Encoding.GetEncoding("GB2312").GetBytes(cardTypeName);

        bw.Write((byte)empIdBytes.Length);
        bw.Write(empIdBytes);
        bw.Write((byte)empNameBytes.Length);
        bw.Write(empNameBytes);
        bw.Write((byte)typeNameBytes.Length);
        bw.Write(typeNameBytes);

        userCode.Data[8] = (byte)keyMode;
        return userCode;
    }
}
