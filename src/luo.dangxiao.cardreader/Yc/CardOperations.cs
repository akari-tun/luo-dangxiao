using luo.dangxiao.cardreader.Yc.Protocol;

namespace luo.dangxiao.cardreader.Yc;

public sealed class CardOperations : IDisposable
{
    private readonly CardReader _reader;

    public CardOperations(CardReader reader)
    {
        _reader = reader ?? throw new ArgumentNullException(nameof(reader));
    }

    public int AuthenticateAndRead(byte sector, byte[] key, byte keyType, byte[] data)
    {
        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;
        if (key == null || key.Length != 6) return (int)ErrorCode.ParameterError;
        if (data == null || data.Length < 16) return (int)ErrorCode.ParameterError;

        int result = _reader.LoadKey(keyType, sector, key);
        if (result != (int)ErrorCode.Success) return result;

        result = _reader.Authentication(keyType, sector);
        if (result != (int)ErrorCode.Success) return result;

        byte blockAddress = (byte)(sector * 4);
        return _reader.Read(blockAddress, data);
    }

    public int AuthenticateAndWrite(byte sector, byte[] key, byte keyType, byte[] data)
    {
        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;
        if (key == null || key.Length != 6) return (int)ErrorCode.ParameterError;
        if (data == null || data.Length != 16) return (int)ErrorCode.ParameterError;

        int result = _reader.LoadKey(keyType, sector, key);
        if (result != (int)ErrorCode.Success) return result;

        result = _reader.Authentication(keyType, sector);
        if (result != (int)ErrorCode.Success) return result;

        byte blockAddress = (byte)(sector * 4);
        return _reader.Write(blockAddress, data);
    }

    public int ReadCardId(out uint serialNumber, out ushort tagType)
    {
        serialNumber = 0;
        tagType = 0;

        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;

        int result = _reader.Request(RequestModes.All, out tagType);
        if (result != (int)ErrorCode.Success) return result;

        result = _reader.Anticoll(0, out serialNumber);
        if (result != (int)ErrorCode.Success) return result;

        return _reader.Select(serialNumber, out _);
    }

    public int ReadBlockWithAuth(byte sector, byte blockOffset, byte[] key, byte keyType, byte[] data)
    {
        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;
        if (key == null || key.Length != 6) return (int)ErrorCode.ParameterError;
        if (blockOffset > 3) return (int)ErrorCode.ParameterError;
        if (data == null || data.Length < 16) return (int)ErrorCode.ParameterError;

        int result = _reader.LoadKey(keyType, sector, key);
        if (result != (int)ErrorCode.Success) return result;

        result = _reader.Authentication(keyType, sector);
        if (result != (int)ErrorCode.Success) return result;

        byte blockAddress = (byte)(sector * 4 + blockOffset);
        return _reader.Read(blockAddress, data);
    }

    public int WriteBlockWithAuth(byte sector, byte blockOffset, byte[] key, byte keyType, byte[] data)
    {
        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;
        if (key == null || key.Length != 6) return (int)ErrorCode.ParameterError;
        if (blockOffset > 3) return (int)ErrorCode.ParameterError;
        if (data == null || data.Length != 16) return (int)ErrorCode.ParameterError;

        int result = _reader.LoadKey(keyType, sector, key);
        if (result != (int)ErrorCode.Success) return result;

        result = _reader.Authentication(keyType, sector);
        if (result != (int)ErrorCode.Success) return result;

        byte blockAddress = (byte)(sector * 4 + blockOffset);
        return _reader.Write(blockAddress, data);
    }

    public int ReadSector(byte sector, byte[] key, byte keyType, byte[][] sectorData)
    {
        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;
        if (key == null || key.Length != 6) return (int)ErrorCode.ParameterError;
        if (sectorData == null || sectorData.Length < 4) return (int)ErrorCode.ParameterError;

        int result = _reader.LoadKey(keyType, sector, key);
        if (result != (int)ErrorCode.Success) return result;

        result = _reader.Authentication(keyType, sector);
        if (result != (int)ErrorCode.Success) return result;

        for (byte i = 0; i < 3; i++)
        {
            sectorData[i] = new byte[16];
            result = _reader.Read((byte)(sector * 4 + i), sectorData[i]);
            if (result != (int)ErrorCode.Success) return result;
        }

        sectorData[3] = new byte[16];
        return _reader.Read((byte)(sector * 4 + 3), sectorData[3]);
    }

    public static byte[] DefaultKeyA => new byte[] { 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 };
    public static byte[] DefaultKeyB => new byte[] { 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5 };
    public static byte[] FactoryKey => new byte[] { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

    public void Dispose()
    {
        _reader.Dispose();
    }
}
