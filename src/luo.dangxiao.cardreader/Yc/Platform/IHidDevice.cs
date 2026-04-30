namespace luo.dangxiao.cardreader.Yc.Platform;

public interface IHidDevice : IDisposable
{
    bool IsOpen { get; }
    ushort VendorId { get; }
    ushort ProductId { get; }
    
    bool Open();
    void Close();
    int Read(byte[] buffer, int timeoutMs = 500);
    int Write(byte[] buffer, int timeoutMs = 500);
}

public interface IHidDeviceEnumerator
{
    IEnumerable<HidDeviceInfo> EnumerateDevices(ushort vendorId, ushort productId);
    HidDeviceInfo? FindDevice(ushort vendorId, ushort productId);
}

public readonly struct HidDeviceInfo
{
    public string DevicePath { get; init; }
    public ushort VendorId { get; init; }
    public ushort ProductId { get; init; }
    public string? Manufacturer { get; init; }
    public string? Product { get; init; }
    public string? SerialNumber { get; init; }
}
