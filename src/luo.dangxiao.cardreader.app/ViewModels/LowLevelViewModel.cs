using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.cardreader.Yc;
using luo.dangxiao.cardreader.Yc.Protocol;

namespace luo.dangxiao.cardreader.app.ViewModels;

public partial class LowLevelViewModel : ViewModelBase
{
    private readonly MainViewModel _main;

    [ObservableProperty] private byte _sector = 1;
    [ObservableProperty] private byte _keyMode = 0x04;
    [ObservableProperty] private string _keyAInput = "FFFFFFFFFFFF";
    [ObservableProperty] private string _keyBInput = "FFFFFFFFFFFF";
    [Obsolete("Use KeyAInput or KeyBInput instead")]
    public string Key { get => KeyAInput; set => KeyAInput = value; }
    [ObservableProperty] private string _block0Data = string.Empty;
    [ObservableProperty] private string _block1Data = string.Empty;
    [ObservableProperty] private string _block2Data = string.Empty;
    [ObservableProperty] private string _block3Data = string.Empty;
    [ObservableProperty] private string _resultMessage = string.Empty;
    [ObservableProperty] private bool _isBusy;
    [ObservableProperty] private ObservableCollection<SectorReadResult> _sectorResults = new();
    [ObservableProperty] private int _selectedSectorTabIndex;

    public LowLevelViewModel(MainViewModel main)
    {
        _main = main;
    }

    public void RefreshKeysFromLicense()
    {
        var license = LicenseManager.Instance;
        var sysInfo = license.SystemInfo;
        if (sysInfo.OperatorPassword != null && sysInfo.OperatorPassword.Length >= 6)
        {
            var keyA = sysInfo.OperatorPassword.AsSpan(0, 6);
            if (!keyA.ToArray().All(b => b == 0))
                KeyAInput = BitConverter.ToString(keyA.ToArray()).Replace("-", "");
        }
        if (sysInfo.SystemKeyB != null && sysInfo.SystemKeyB.Length >= 6)
        {
            var keyB = sysInfo.SystemKeyB.AsSpan(0, 6);
            if (!keyB.ToArray().All(b => b == 0))
                KeyBInput = BitConverter.ToString(keyB.ToArray()).Replace("-", "");
        }
    }

    private byte[]? GetActiveKey()
    {
        var hex = (KeyMode & 0x04) != 0 ? KeyBInput : KeyAInput;
        return HexStringToBytes(hex);
    }

    [RelayCommand]
    private void ReadBlock()
    {
        if (_main.Reader == null) return;
        IsBusy = true;
        try
        {
            var keyA = HexStringToBytes(KeyAInput);
            var keyB = HexStringToBytes(KeyBInput);
            if (keyA == null || keyB == null) { ResultMessage = "Invalid key format"; return; }

            _main.Reader.Halt();
            var cardResult = _main.Reader.Card(0x52, out _);
            if (cardResult != 0) { ResultMessage = "No card detected"; return; }

            byte slot = (byte)(KeyMode & 0x03);
            byte useKeyMode = (byte)(slot | (KeyMode & 0x04));
            byte[] useKey = (KeyMode & 0x04) != 0 ? keyB : keyA;

            var loadResult = _main.Reader.LoadKey((byte)(useKeyMode | KeyTypes.KeySet2), Sector, useKey);
            if (loadResult != 0)
            {
                ResultMessage = $"Load key failed: {MainViewModel.GetErrorMessage(loadResult)}";
                return;
            }

            var authResult = _main.Reader.Authentication((byte)(useKeyMode | KeyTypes.KeySet2), Sector);
            if (authResult != 0)
            {
                ResultMessage = $"Authentication failed: {MainViewModel.GetErrorMessage(authResult)}";
                return;
            }

            var blockResults = new string[4];
            bool allOk = true;
            for (int bo = 0; bo < 4; bo++)
            {
                byte addr = (byte)(Sector * 4 + bo);
                byte[] data = new byte[16];
                int rr = _main.Reader.Read(addr, data);
                if (rr == 0)
                {
                    blockResults[bo] = BitConverter.ToString(data).Replace("-", " ");
                }
                else
                {
                    blockResults[bo] = $"ERR: {MainViewModel.GetErrorMessage(rr)}";
                    allOk = false;
                }
            }

            Block0Data = blockResults[0];
            Block1Data = blockResults[1];
            Block2Data = blockResults[2];
            Block3Data = blockResults[3];
            ResultMessage = allOk ? $"Sector {Sector} read OK" : $"Sector {Sector} read completed with errors";
        }
        finally { IsBusy = false; }
    }

    [RelayCommand]
    private void WriteBlock()
    {
        if (_main.Reader == null) return;
        IsBusy = true;
        try
        {
            var cardResult = _main.Reader.Card(0x52, out _);
            if (cardResult != 0) { ResultMessage = "No card detected"; return; }

            var key = GetActiveKey();
            if (key == null) { ResultMessage = "Invalid key format"; return; }

            var blockInputs = new[] { Block0Data, Block1Data, Block2Data, Block3Data };
            var blockBytes = new byte[4][];
            for (int i = 0; i < 4; i++)
            {
                var data = HexStringToBytes(blockInputs[i]);
                if (data == null || data.Length != 16)
                {
                    ResultMessage = $"Block {i} invalid format (need exactly 16 hex bytes, got {data?.Length ?? 0})";
                    return;
                }
                blockBytes[i] = data;
            }

            byte slot = (byte)(KeyMode & 0x03);
            byte useKeyMode = (byte)(slot | (KeyMode & 0x04));

            var loadResult = _main.Reader.LoadKey(useKeyMode, Sector, key);
            if (loadResult != 0) { ResultMessage = $"Load key failed: {MainViewModel.GetErrorMessage(loadResult)}"; return; }
            var authResult = _main.Reader.Authentication(useKeyMode, Sector);
            if (authResult != 0) { ResultMessage = $"Authentication failed: {MainViewModel.GetErrorMessage(authResult)}"; return; }

            bool allOk = true;
            for (int i = 0; i < 4; i++)
            {
                byte addr = (byte)(Sector * 4 + i);
                int wr = _main.Reader.Write(addr, blockBytes[i]);
                if (wr != 0)
                {
                    ResultMessage = $"Block {i} write failed: {MainViewModel.GetErrorMessage(wr)}";
                    allOk = false;
                    break;
                }
            }
            ResultMessage = allOk ? $"Sector {Sector} write OK" : $"Sector {Sector} write completed with errors";
        }
        finally { IsBusy = false; }
    }

    [RelayCommand]
    private async Task ReadAllSectorsAsync()
    {
        if (_main.Reader == null) return;
        IsBusy = true;
        SectorResults.Clear();
        SelectedSectorTabIndex = 0;
        var keyA = HexStringToBytes(KeyAInput);
        var keyB = HexStringToBytes(KeyBInput);
        if (keyA == null || keyB == null) { ResultMessage = "Invalid key format"; IsBusy = false; return; }

        var cardResult = _main.Reader.Card(0x52, out _);
        if (cardResult != 0) { ResultMessage = $"Card detect failed: {MainViewModel.GetErrorMessage(cardResult)}"; IsBusy = false; return; }

        ResultMessage = "Card detected. Reading block 0...";

        var block0 = new byte[16];
        var block0Result = _main.Reader.Read(0, block0);

        byte slot = (byte)(KeyMode & 0x03);
        byte useKeyMode = (byte)(slot | KeyMode & 0x04);
        byte[] useKey = (KeyMode & 0x04) != 0 ? keyB : keyA;

        try
        {
            _main.Reader.Halt();
            _main.Reader.Card(0x52, out _);

            for (byte s = 0; s < 16; s++)
            {
                var sr = new SectorReadResult { Sector = s };
                string st;

                if (s == 0)
                {
                    sr.Block0Result = block0Result == 0
                        ? BitConverter.ToString(block0).Replace("-", " ")
                        : $"ERR: {MainViewModel.GetErrorMessage(block0Result)}";
                }

                var loadResult = _main.Reader.LoadKey(useKeyMode, s, useKey);
                if (loadResult != 0)
                {
                    st = $"Load key ERR: {MainViewModel.GetErrorMessage(loadResult)}";
                    sr.Status = st;
                    if (s > 0) sr.Block0Result = st;
                    sr.Block1Result = st; sr.Block2Result = st; sr.Block3Result = st;
                    SectorResults.Add(sr);
                    _main.Reader.Halt();
                    _main.Reader.Card(0x52, out _);
                    continue;
                }

                var authResult = _main.Reader.Authentication(useKeyMode, s);
                if (authResult != 0)
                {
                    st = $"Auth ERR: {MainViewModel.GetErrorMessage(authResult)}";
                    sr.Status = st;
                    sr.Block1Result = st; sr.Block2Result = st; sr.Block3Result = st;
                    SectorResults.Add(sr);
                    _main.Reader.Halt();
                    _main.Reader.Card(0x52, out _);
                    continue;
                }

                for (int bo = 0; bo < 4; bo++)
                {
                    byte ba = (byte)(s * 4 + bo);
                    byte[] d = new byte[16];
                    int rr = _main.Reader.Read(ba, d);
                    string hd = rr == 0
                        ? BitConverter.ToString(d).Replace("-", " ")
                        : $"ERR: {MainViewModel.GetErrorMessage(rr)}";
                    switch (bo) { case 0: sr.Block0Result = hd; break; case 1: sr.Block1Result = hd; break; case 2: sr.Block2Result = hd; break; case 3: sr.Block3Result = hd; break; }
                }
                sr.Status = "OK";
                SectorResults.Add(sr);
            }
            ResultMessage = "All 16 sectors read completed";
        }
        catch (Exception ex) { ResultMessage = $"Error: {ex.Message}"; }
        finally { _main.Reader.Halt(); IsBusy = false; }
    }

    private static byte[]? HexStringToBytes(string hex)
    {
        try
        {
            hex = hex.Replace(" ", "").Replace("-", "");
            if (hex.Length % 2 != 0) return null;
            byte[] bytes = new byte[hex.Length / 2];
            for (int i = 0; i < bytes.Length; i++) { bytes[i] = Convert.ToByte(hex.Substring(i * 2, 2), 16); }
            return bytes;
        }
        catch { return null; }
    }
}
