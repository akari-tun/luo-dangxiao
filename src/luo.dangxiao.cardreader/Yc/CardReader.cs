using luo.dangxiao.cardreader.Yc.Platform;
using luo.dangxiao.cardreader.Yc.Protocol;

namespace luo.dangxiao.cardreader.Yc;

public sealed class CardReader : IDisposable
{
    private readonly IHidDevice _device;
    private bool _disposed;

    public CardReader(ushort vendorId = UsbIdentifiers.VendorId, ushort productId = UsbIdentifiers.ProductId)
    {
        _device = HidDeviceFactory.Create(vendorId, productId);
    }

    public CardReader(string devicePath)
    {
        _device = HidDeviceFactory.Create(devicePath);
    }

    public bool IsOpen => _device.IsOpen;

    public bool Open()
    {
        if (_disposed) throw new ObjectDisposedException(nameof(CardReader));
        return _device.Open();
    }

    public void Close()
    {
        _device.Close();
    }

    public int Init(int port = 0, int baud = 0)
    {
        return Open() ? (int)ErrorCode.Success : (int)ErrorCode.CommunicationError;
    }

    public int Exit()
    {
        Close();
        return (int)ErrorCode.Success;
    }

    public int Beep(ushort durationMs = 10)
    {
        if (!IsOpen) return (int)ErrorCode.CommunicationError;

        byte[] data = new byte[4];
        data[0] = 0x00;
        data[1] = 0x01;
        data[2] = (byte)(durationMs & 0xFF);
        data[3] = (byte)((durationMs >> 8) & 0xFF);

        return SendCommand(ControlWords.SetBellCode, ControlWords.SetBell, data);
    }

    public int Request(byte mode, out ushort tagType)
    {
        tagType = 0;
        if (!IsOpen) return (int)ErrorCode.CommunicationError;

        OpenRf();

        byte[] data = new byte[1];
        data[0] = mode == 0 ? (byte)0x26 : (byte)0x52;

        int result = SendCommandWithResponse(ControlWords.RequestCode, ControlWords.Request, data, out byte[]? response);
        
        if (result == (int)ErrorCode.Success && response != null && response.Length >= 2)
        {
            tagType = (ushort)(response[0] | (response[1] << 8));
        }

        return result;
    }

    public int Anticoll(byte bcnt, out uint serialNumber)
    {
        serialNumber = 0;
        if (!IsOpen) return (int)ErrorCode.CommunicationError;

        byte[] data = new byte[1];
        data[0] = bcnt;

        int result = SendCommandWithResponse(ControlWords.AnticollCode, ControlWords.Anticoll, data, out byte[]? response);
        
        if (result == (int)ErrorCode.Success && response != null && response.Length >= 4)
        {
            serialNumber = BitConverter.ToUInt32(response, 0);
        }

        return result;
    }

    public int Select(uint serialNumber, out byte size)
    {
        size = 0;
        if (!IsOpen) return (int)ErrorCode.CommunicationError;

        byte[] data = new byte[8];
        BitConverter.TryWriteBytes(data.AsSpan(0, 4), serialNumber);

        int result = SendCommandWithResponse(ControlWords.SelectCode, ControlWords.Select, data, out byte[]? response);
        
        if (result == (int)ErrorCode.Success && response != null && response.Length >= 1)
        {
            size = response[^1];
        }

        return result;
    }

    public int Authentication(byte mode, byte sector)
    {
        if (!IsOpen) return (int)ErrorCode.CommunicationError;

        byte[] data = new byte[2];
        data[0] = mode;
        data[1] = sector;

        return SendCommand(ControlWords.AuthenticationCode, ControlWords.Authentication, data);
    }

    public int LoadKey(byte mode, byte sector, byte[] key)
    {
        if (!IsOpen) return (int)ErrorCode.CommunicationError;
        if (key == null || key.Length != 6) return (int)ErrorCode.ParameterError;

        byte[] data = new byte[8];
        data[0] = mode;
        data[1] = sector;
        Buffer.BlockCopy(key, 0, data, 2, 6);

        return SendCommand(ControlWords.LoadKeyCode, ControlWords.LoadKey, data);
    }

    public int Read(byte blockAddress, byte[] data)
    {
        if (!IsOpen) return (int)ErrorCode.CommunicationError;
        if (data == null || data.Length < 16) return (int)ErrorCode.ParameterError;

        byte[] sendData = new byte[1];
        sendData[0] = blockAddress;

        int result = SendCommandWithResponse(ControlWords.ReadCode, ControlWords.Read, sendData, out byte[]? response);
        
        if (result == (int)ErrorCode.Success && response != null)
        {
            int copyLen = Math.Min(response.Length, 16);
            Buffer.BlockCopy(response, 0, data, 0, copyLen);
        }

        return result;
    }

    public int Write(byte blockAddress, byte[] data)
    {
        if (!IsOpen) return (int)ErrorCode.CommunicationError;
        if (data == null || data.Length != 16) return (int)ErrorCode.ParameterError;

        byte[] sendData = new byte[17];
        sendData[0] = blockAddress;
        Buffer.BlockCopy(data, 0, sendData, 1, 16);

        return SendCommand(ControlWords.WriteCode, ControlWords.Write, sendData);
    }

    public int Halt()
    {
        if (!IsOpen) return (int)ErrorCode.CommunicationError;
        return SendCommand(ControlWords.HaltCode, ControlWords.Halt, null);
    }

    public int Card(byte mode, out uint serialNumber)
    {
        serialNumber = 0;

        int result = Request(mode, out ushort tagType);
        if (result != (int)ErrorCode.Success) return result;

        result = Anticoll(0, out serialNumber);
        if (result != (int)ErrorCode.Success) return result;

        result = Select(serialNumber, out _);
        return result;
    }

    public int SetTime(byte[] time)
    {
        if (!IsOpen) return (int)ErrorCode.CommunicationError;
        if (time == null || time.Length != 7) return (int)ErrorCode.ParameterError;

        return SendCommand(ControlWords.SetTimeCode, ControlWords.SetTime, time);
    }

    public int GetTime(byte[] time)
    {
        if (!IsOpen) return (int)ErrorCode.CommunicationError;
        if (time == null || time.Length < 7) return (int)ErrorCode.ParameterError;

        int result = SendCommandWithResponse(ControlWords.GetTimeCode, ControlWords.GetTime, null, out byte[]? response);
        
        if (result == (int)ErrorCode.Success && response != null)
        {
            int copyLen = Math.Min(response.Length, 7);
            Buffer.BlockCopy(response, 0, time, 0, copyLen);
        }

        return result;
    }

    private int OpenRf()
    {
        byte[] sendBuffer = PacketBuilder.BuildPacket(
            ProtocolCommands.DeviceAddress,
            ControlWords.OpenRfCode,
            ControlWords.OpenRf,
            null);

        ReaderLogger.Log($"[OpenRf] Sending {sendBuffer.Length} bytes: {BitConverter.ToString(sendBuffer)}");

        int written = _device.Write(sendBuffer, 10);
        if (written <= 0) return (int)ErrorCode.WriteError;

        byte[] receiveBuffer = new byte[64];
        int read = _device.Read(receiveBuffer, 10);
        
        if (read <= 0) return (int)ErrorCode.WriteError;

        ReaderLogger.Log($"[OpenRf] Received {read} bytes: {BitConverter.ToString(receiveBuffer, 0, Math.Min(read, 20))}");

        return PacketBuilder.ParsePacket(receiveBuffer, ProtocolCommands.DeviceAddress, ControlWords.OpenRfCode, out _);
    }

    private int SendCommand(byte controlCode, string commandName, byte[]? data)
    {
        byte[] sendBuffer = PacketBuilder.BuildPacket(
            ProtocolCommands.DeviceAddress,
            controlCode,
            commandName,
            data);

        ReaderLogger.Log($"[SendCommand:{commandName}] Sending {sendBuffer.Length} bytes: {BitConverter.ToString(sendBuffer)}");

        int written = _device.Write(sendBuffer, 500);
        if (written <= 0) return (int)ErrorCode.WriteError;

        byte[] receiveBuffer = new byte[64];
        int read = _device.Read(receiveBuffer, 500);
        
        if (read <= 0) return (int)ErrorCode.WriteError;

        ReaderLogger.Log($"[SendCommand:{commandName}] Received {read} bytes: {BitConverter.ToString(receiveBuffer, 0, Math.Min(read, 20))}");

        return PacketBuilder.ParsePacket(receiveBuffer, ProtocolCommands.DeviceAddress, controlCode, out _);
    }

    private int SendCommandWithResponse(byte controlCode, string commandName, byte[]? data, out byte[]? responseData)
    {
        responseData = null;

        byte[] sendBuffer = PacketBuilder.BuildPacket(
            ProtocolCommands.DeviceAddress,
            controlCode,
            commandName,
            data);

        int written = _device.Write(sendBuffer, 500);
        if (written <= 0) return (int)ErrorCode.WriteError;

        byte[] receiveBuffer = new byte[64];
        int read = _device.Read(receiveBuffer, 500);
        
        if (read <= 0) return (int)ErrorCode.WriteError;

        return PacketBuilder.ParsePacket(receiveBuffer, ProtocolCommands.DeviceAddress, controlCode, out responseData);
    }

    public static IEnumerable<HidDeviceInfo> EnumerateDevices(ushort vendorId = 0, ushort productId = 0)
    {
        return HidDeviceFactory.EnumerateDevices(vendorId, productId);
    }

    public void Dispose()
    {
        if (_disposed) return;
        Close();
        _device.Dispose();
        _disposed = true;
    }
}
