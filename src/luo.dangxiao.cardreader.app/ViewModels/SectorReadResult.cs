namespace luo.dangxiao.cardreader.app.ViewModels;

public class SectorReadResult
{
    public int Sector { get; set; }
    public string Status { get; set; } = "PENDING";
    public string Block0Result { get; set; } = string.Empty;
    public string Block1Result { get; set; } = string.Empty;
    public string Block2Result { get; set; } = string.Empty;
    public string Block3Result { get; set; } = string.Empty;
}
