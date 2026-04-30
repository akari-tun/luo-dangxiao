using System.Security.Cryptography;
using luo.dangxiao.cardreader.Yc.Structs;

namespace luo.dangxiao.cardreader.Yc;

public class LicenseManager
{
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

            // Decrypt system info at offset 0x308 (776)
            // System info is 120 bytes, encrypted in 15 blocks of 8 bytes each using DES
            byte[] encryptedSysInfo = new byte[120];
            Buffer.BlockCopy(licenceData, 0x308, encryptedSysInfo, 0, 120);

            byte[] decryptedSysInfo = new byte[120];
            byte[] key12 = System.Text.Encoding.ASCII.GetBytes("20060110");

            // Decrypt each 8-byte block using standard DES
            using (var des = DES.Create())
            {
                des.Key = key12;
                des.IV = new byte[8]; // Zero IV
                des.Mode = CipherMode.ECB;
                des.Padding = PaddingMode.None;

                using (var decryptor = des.CreateDecryptor())
                {
                    for (int i = 0; i < 15; i++)
                    {
                        byte[] block = new byte[8];
                        Buffer.BlockCopy(encryptedSysInfo, i * 8, block, 0, 8);
                        
                        byte[] decryptedBlock = decryptor.TransformFinalBlock(block, 0, 8);
                        Buffer.BlockCopy(decryptedBlock, 0, decryptedSysInfo, i * 8, 8);
                    }
                }
            }

            // Parse system info from decrypted data
            var sysInfo = ParseSystemInfo(decryptedSysInfo);
            
            // Set system info and update auth flags
            SetSystemInfo(sysInfo);
            
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
}
