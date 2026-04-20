namespace luo.dangxiao.models
{
    public sealed class YktApiConfig
    {
        public string BaseUrl { get; set; } = string.Empty;

        public int TimeoutSeconds { get; set; } = 30;
    }
}
