using System.Runtime.InteropServices;

namespace luo.dangxiao.cardreader.Yc.Structs;

public struct SystemInfo
{
    public byte[] Reserved;
    public int AccessControlSystem;
    public int AccessControlSector;
    public byte AccessControlCardType;
    public int PaymentSystem;
    public int PaymentSector;
    public byte[] SystemCardNumber;
    public byte[] SystemKeyB;
    public byte[] UserPassword;
    public byte[] OperatorPassword;
    public byte[] CommunicationPassword;
    public byte[] Reserved2;

    public SystemInfo()
    {
        Reserved = new byte[2];
        SystemCardNumber = new byte[5];
        SystemKeyB = new byte[8];
        UserPassword = new byte[8];
        OperatorPassword = new byte[8];
        CommunicationPassword = new byte[8];
        Reserved2 = new byte[8];
    }
}

[StructLayout(LayoutKind.Sequential, Pack = 1, Size = 120)]
public struct SystemInfoNew
{
    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
    public byte[] Reserved;
    public int AccessControlSystem;
    public int AccessControlSector;
    public byte AccessControlCardType;
    public int PaymentSystem;
    public int PaymentSector;
    public int WaterBillingSystem;
    public int WaterBillingSector;
    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 5)]
    public byte[] SystemCardNumberPayment;
    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 5)]
    public byte[] SystemCardNumberAccess;
    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 5)]
    public byte[] SystemCardNumberWater;
    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
    public byte[] SystemKeyB;
    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
    public byte[] UserPassword;
    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
    public byte[] OperatorPassword;
    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
    public byte[] CommunicationPassword;
    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
    public byte[] Reserved2;
    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
    public byte[] UserKeyAB;
    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
    public byte[] UserReturnKeyAB;

    public SystemInfoNew()
    {
        Reserved = new byte[2];
        SystemCardNumberPayment = new byte[5];
        SystemCardNumberAccess = new byte[5];
        SystemCardNumberWater = new byte[5];
        SystemKeyB = new byte[8];
        UserPassword = new byte[8];
        OperatorPassword = new byte[8];
        CommunicationPassword = new byte[8];
        Reserved2 = new byte[6];
        UserKeyAB = new byte[16];
        UserReturnKeyAB = new byte[16];
    }
}

public struct UserCardInfo
{
    public uint CardNumber;
    public uint RestMoney;
    public uint UsedMoney;
    public uint UsedRechargeTimes;
    public uint UsedAllTimes;
    public uint UsedAllMoney;
    public uint UsedAddress;
    public uint CardType;
    public byte[] UserPassword;
    public byte[] UserEndTime;
    public byte[] UserName;
    public byte[] UsedTime;
    public byte[] UserCardData;

    public UserCardInfo()
    {
        UserPassword = new byte[3];
        UserEndTime = new byte[3];
        UserName = new byte[6];
        UsedTime = new byte[6];
        UserCardData = new byte[48];
    }
}

public struct AdvancedMessage
{
    public uint CardNumber;
    public uint CardType;
    public uint RestMoney;
    public uint RechargeTimes;
    public uint UsedAllTimes;
    public uint UsedAllMoney;
    public uint UsedMoney;
    public byte[] UserPassword;
    public byte[] UsedTime;
    public byte[] UserName;
    public byte[] UserValidityPeriod;
    public byte[] KqName;
    public byte[] KqDepartment;
    public byte[] KqNumber;

    public AdvancedMessage()
    {
        UserPassword = new byte[4];
        UsedTime = new byte[6];
        UserName = new byte[20];
        UserValidityPeriod = new byte[4];
        KqName = new byte[48];
        KqDepartment = new byte[48];
        KqNumber = new byte[48];
    }
}

public struct RecordData
{
    public int MachineNo;
    public uint TotalBalance;
}

public struct RecordDetailedData
{
    public uint CardNumber;
    public uint CardRestMoney;
    public uint CardUsedMoney;
    public uint CardUsedTimes;
    public uint CardAddress;
    public byte[] CardTimes;

    public RecordDetailedData()
    {
        CardTimes = new byte[12];
    }
}

public struct RecordAllData
{
    public uint MaterielAllMoney;
    public uint MaterielAllTimes;
    public uint MaterielAllAmount;
    public uint CardAddress;
}

public struct JsFeeRate
{
    public byte ConsumeMode;
    public byte MinFeeUnit;
    public uint Rate1;
    public uint Rate2;
    public uint Rate3;
    public uint Rate4;
    public uint Rate5;
    public uint Rate6;
    public uint Rate7;
    public uint Rate8;
}

public struct JsAdvanPara
{
    public byte GradeRate1;
    public byte GradeRate2;
    public byte GradeRate3;
    public ushort OneGrade;
    public ushort TwoGrade;
    public ushort MaxConsumeTimes;
    public uint MaxConsumeMoney;
    public uint FreeTime;
    public ushort ConsumeInterval;
    public ushort OnceMaxConsumeMoney;
    public byte EnCardType;
    public byte UsedOneCardMode;
    public byte EnAutoClose;
    public byte UserCardFlag;
    public byte AutoWriteRecordFlag;
    public byte AutoLockUsedCard;
    public byte UsedCardOverdraftFlag;
    public byte UsedCardOverdraftFlag2;
    public byte GkCalaKey;
}

public struct XfRecord
{
    public uint RecordSnr;
    public uint CardNum;
    public uint RestMoney;
    public uint UsedMoney;
    public uint UsedAllTimes;
    public uint TerminalId;
    public uint Operator;
    public byte[] UsedTime;
    public byte Status;

    public XfRecord()
    {
        UsedTime = new byte[12];
    }
}

public struct Ycxfg30UserRecordInfo
{
    public uint RecordSnr;
    public uint CardNum;
    public uint RestMoney;
    public uint UsedMoney;
    public uint UsedAllTimes;
    public uint TerminalId;
    public uint OldRestMoney;
    public uint GetKeyMoney;
    public ushort Operator;
    public byte[] UsedTime;
    public byte[] CardUID;
    public byte Step;
    public byte Status;
    public byte CardType;
    public byte Crc;

    public Ycxfg30UserRecordInfo()
    {
        UsedTime = new byte[6];
        CardUID = new byte[4];
    }
}
