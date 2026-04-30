// ================================================================================
// YCCARD Protocol Packet Builder
// HID Protocol Packet Construction and Parsing
// ================================================================================

using System;
using System.Security.Cryptography;
using luo.dangxiao.cardreader.Yc;

namespace luo.dangxiao.cardreader.Yc.Protocol;

/// <summary>
/// Implements the HID protocol packet construction and parsing
/// compatible with the original YCCARD C++ library
/// </summary>
public static class PacketBuilder
{
    private static readonly RandomNumberGenerator _rng = RandomNumberGenerator.Create();

    /// <summary>
    /// Builds a protocol packet for transmission
    /// </summary>
    /// <param name="address">Device address</param>
    /// <param name="controlWord">Control word/command code</param>
    /// <param name="commandName">Command name string</param>
    /// <param name="data">Data payload</param>
    /// <returns>Complete packet ready for transmission</returns>
    public static byte[] BuildPacket(ushort address, byte controlWord, string commandName, byte[]? data = null)
    {
        int dataLen = data?.Length ?? 0;
        int commandLen = commandName.Length;

        // Calculate total length
        // Header(2) + Address(2) + Control(1) + Length(1) + CommandName + Data + CRC(1) + Tail(2)
        int totalLen = 2 + 2 + 1 + 1 + commandLen + dataLen + 1 + 2;

        Span<byte> packet = stackalloc byte[totalLen];
        int pos = 0;

        // Header
        packet[pos++] = (byte)(ProtocolCommands.CommandStart & 0xFF);
        packet[pos++] = (byte)((ProtocolCommands.CommandStart >> 8) & 0xFF);

        // Address
        packet[pos++] = (byte)(address & 0xFF);
        packet[pos++] = (byte)((address >> 8) & 0xFF);

        // Control word
        packet[pos++] = controlWord;

        // Data length (command name + data)
        packet[pos++] = (byte)(commandLen + dataLen);

        // Command name
        for (int i = 0; i < commandLen; i++)
        {
            packet[pos++] = (byte)commandName[i];
        }

        // Data
        if (data != null && dataLen > 0)
        {
            data.CopyTo(packet.Slice(pos, dataLen));
            pos += dataLen;
        }

        // CRC8 (from address to before CRC)
        byte crc = CalculateCrc8(packet.Slice(2, pos - 2));
        packet[pos++] = crc;

        // Tail
        packet[pos++] = (byte)(ProtocolCommands.CommandEnd & 0xFF);
        packet[pos++] = (byte)((ProtocolCommands.CommandEnd >> 8) & 0xFF);

        // Apply XOR obfuscation
        return ObfuscatePacket(packet.ToArray());
    }

    /// <summary>
    /// Parses a received protocol packet
    /// </summary>
    /// <param name="packet">Raw received packet</param>
    /// <param name="expectedAddress">Expected device address</param>
    /// <param name="expectedControlWord">Expected control word</param>
    /// <param name="data">Output data payload</param>
    /// <returns>Error code (0 = success)</returns>
    public static int ParsePacket(byte[] packet, ushort expectedAddress, byte expectedControlWord, out byte[]? data)
    {
        data = null;

        // De-obfuscate packet
        byte[] decoded = DeobfuscatePacket(packet);

        // Log decrypted data for debugging
        ReaderLogger.Log($"[ParsePacket] Received {packet.Length} bytes");
        ReaderLogger.Log($"[ParsePacket] Raw (full): {BitConverter.ToString(packet, 0, Math.Min(packet.Length, 64))}");
        ReaderLogger.Log($"[ParsePacket] Decoded ({decoded.Length} bytes): {BitConverter.ToString(decoded)}");
        ReaderLogger.Log($"[ParsePacket] Expected: Addr={expectedAddress:X4}, Ctrl={expectedControlWord:X2}");

        int pos = 0;

        // Check header
        if (decoded.Length < 8)
        {
            ReaderLogger.Log($"[ParsePacket] ERROR: Decoded length {decoded.Length} < 8");
            return (int)ErrorCode.ReceiveDataError;
        }

        ushort header = (ushort)(decoded[0] | (decoded[1] << 8));
        ReaderLogger.Log($"[ParsePacket] Header: 0x{header:X4}, Expected: 0x{ProtocolCommands.CommandStart:X4}");
        if (header != ProtocolCommands.CommandStart)
            return (int)ErrorCode.CommandStartError;
        pos += 2;

        // Check address
        ushort address = (ushort)(decoded[pos] | (decoded[pos + 1] << 8));
        ReaderLogger.Log($"[ParsePacket] Address: 0x{address:X4}, Expected: 0x{expectedAddress:X4}");
        if (address != expectedAddress)
            return (int)ErrorCode.DeviceAddressError;
        pos += 2;

        // Check control word
        // Device responds with control word + 0x55 (observed in Linux logs)
        byte expectedResponseCtrl = (byte)(expectedControlWord + 0x55);
        ReaderLogger.Log($"[ParsePacket] Control: 0x{decoded[pos]:X2}, Expected: 0x{expectedControlWord:X2} or 0x{expectedResponseCtrl:X2}");
        if (decoded[pos] != expectedControlWord && decoded[pos] != expectedResponseCtrl)
            return (int)ErrorCode.ControlWordError;
        pos += 1;

        // Get data length
        int dataLen = decoded[pos];
        pos += 1;

        if (dataLen == 0)
            return (int)ErrorCode.ReceiveLengthError;

        // Check status byte (first byte of response data)
        if (decoded[pos] != 0)
            return decoded[pos]; // Return the status/error code
        pos += 1;

        // Extract data (excluding status byte)
        int actualDataLen = dataLen - 1;
        if (actualDataLen > 0)
        {
            data = new byte[actualDataLen];
            Buffer.BlockCopy(decoded, pos, data, 0, actualDataLen);
            pos += actualDataLen;
        }

        // Verify CRC
        byte expectedCrc = CalculateCrc8(decoded, 2, pos - 2);
        if (decoded[pos] != expectedCrc)
            return (int)ErrorCode.CrcError;
        pos += 1;

        // Check tail
        ushort tail = (ushort)(decoded[pos] | (decoded[pos + 1] << 8));
        ReaderLogger.Log($"[ParsePacket] Tail: 0x{tail:X4} at pos {pos}, Expected: 0x{ProtocolCommands.CommandEnd:X4}");
        if (tail != ProtocolCommands.CommandEnd)
        {
            ReaderLogger.Log($"[ParsePacket] ERROR: Tail mismatch! Got {decoded[pos]:X2}-{decoded[pos+1]:X2}, expected 6B-BB");
            return (int)ErrorCode.CommandEndError;
        }

        return (int)ErrorCode.Success;
    }

    /// <summary>
    /// Applies XOR obfuscation to packet (matching original C++ implementation)
    /// </summary>
    private static byte[] ObfuscatePacket(byte[] packet)
    {
        // Generate random values for XOR keys
        Span<byte> randBytes = stackalloc byte[4];
        _rng.GetBytes(randBytes);

        int rand1 = randBytes[0] | (randBytes[1] << 8);
        int rand2Raw = randBytes[2] | (randBytes[3] << 8);
        int rand2 = rand1 ^ rand2Raw;

        // The C++ implementation XORs all bytes, and the receiver skips positions 6-7
        // So we XOR all bytes and the result will be received correctly
        byte[] result = new byte[packet.Length + 2];

        result[0] = (byte)((rand1 + 0x05) & 0xFF);
        result[1] = (byte)((rand2 + 0x0A) & 0xFF);

        for (int i = 0; i < packet.Length; i++)
        {
            result[i + 2] = (byte)((packet[i] ^ rand1) ^ rand2);
        }

        return result;
    }

    /// <summary>
    /// Removes XOR obfuscation from packet
    /// Matches the exact C++ UNPackage algorithm
    /// </summary>
    private static byte[] DeobfuscatePacket(byte[] packet)
    {
        if (packet.Length < 2)
            return packet;

        int rand1 = (packet[0] - 0x05) & 0xFF;
        int rand2 = (packet[1] - 0x0A) & 0xFF;

        // Log the decryption keys
        ReaderLogger.Log($"[Deobfuscate] Keys: rand1=0x{rand1:X2}, rand2=0x{rand2:X2}");
        ReaderLogger.Log($"[Deobfuscate] Packet length: {packet.Length}");
        
        int maxDecodedLen = packet.Length - 2;
        byte[] result = new byte[maxDecodedLen];

        // Decrypt first 6 bytes
        int firstBlockLen = Math.Min(6, maxDecodedLen);
        for (int i = 0; i < firstBlockLen; i++)
        {
            result[i] = (byte)((packet[i + 2] ^ rand1) ^ rand2);
        }
        
        ReaderLogger.Log($"[Deobfuscate] First 6 decrypted: {BitConverter.ToString(result, 0, Math.Min(6, result.Length))}");

        // Decrypt remaining bytes
        if (maxDecodedLen > 6)
        {
            int dataLen = result[5];
            int totalDecodedLen = dataLen + 9;
            ReaderLogger.Log($"[Deobfuscate] dataLen={dataLen}, totalDecodedLen={totalDecodedLen}");
            
            int remainingLen = Math.Min(totalDecodedLen - 6, maxDecodedLen - 6);
            
            for (int i = 0; i < remainingLen; i++)
            {
                int encryptedPos = 8 + i;
                if (encryptedPos < packet.Length)
                {
                    result[6 + i] = (byte)((packet[encryptedPos] ^ rand1) ^ rand2);
                }
            }
            
            if (totalDecodedLen > 0 && totalDecodedLen < maxDecodedLen)
            {
                ReaderLogger.Log($"[Deobfuscate] Resizing result from {result.Length} to {totalDecodedLen}");
                Array.Resize(ref result, totalDecodedLen);
            }
        }

        ReaderLogger.Log($"[Deobfuscate] Returning {result.Length} bytes: {BitConverter.ToString(result, 0, Math.Min(result.Length, 20))}");
        return result;
    }

    /// <summary>
    /// Calculates CRC8 checksum
    /// </summary>
    public static byte CalculateCrc8(ReadOnlySpan<byte> data)
    {
        byte crc = 0;
        for (int i = 0; i < data.Length; i++)
        {
            crc ^= data[i];
        }
        return (byte)(~crc);
    }

    /// <summary>
    /// Calculates CRC8 with offset and length
    /// </summary>
    public static byte CalculateCrc8(byte[] data, int offset, int length)
    {
        return CalculateCrc8(new ReadOnlySpan<byte>(data, offset, length));
    }

    /// <summary>
    /// Calculates CRC16 (CRC-CCITT)
    /// </summary>
    public static ushort CalculateCrc16(ReadOnlySpan<byte> data)
    {
        const ushort Preset = 0xFFFF;
        const ushort Polynomial = 0x1021;

        ushort crc = Preset;
        for (int i = 0; i < data.Length; i++)
        {
            crc ^= (ushort)(data[i] << 8);
            for (int j = 0; j < 8; j++)
            {
                if ((crc & 0x8000) != 0)
                    crc = (ushort)((crc << 1) ^ Polynomial);
                else
                    crc <<= 1;
            }
        }
        return crc;
    }
}
