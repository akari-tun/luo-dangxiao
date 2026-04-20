using System.IO;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace luo.dangxiao.models
{
    public abstract class ConfigModel
    {
        private static readonly JsonSerializerOptions s_jsonSerializerOptions = new()
        {
            WriteIndented = true,
            PropertyNameCaseInsensitive = true,
            Converters =
            {
                new JsonStringEnumConverter()
            }
        };

        public string Theme { get; set; } = "Light";

        public YktApiConfig YktApiConfig { get; set; } = new();

        public static T Load<T>(string? configFilePath = null) where T : ConfigModel, new()
        {
            var path = ResolveConfigFilePath(configFilePath);

            if (!File.Exists(path))
            {
                var defaultConfig = new T();
                Save(path, defaultConfig);
                return defaultConfig;
            }

            try
            {
                var json = File.ReadAllText(path);
                var config = JsonSerializer.Deserialize<T>(json, s_jsonSerializerOptions);
                if (config is not null)
                {
                    return config;
                }
            }
            catch
            {
            }

            var fallbackConfig = new T();
            Save(path, fallbackConfig);
            return fallbackConfig;
        }

        private static string ResolveConfigFilePath(string? configFilePath)
        {
            if (string.IsNullOrWhiteSpace(configFilePath))
            {
                return Path.Combine(AppContext.BaseDirectory, "config.json");
            }

            return Path.IsPathRooted(configFilePath)
                ? configFilePath
                : Path.Combine(AppContext.BaseDirectory, configFilePath);
        }

        private static void Save<T>(string path, T config) where T : ConfigModel
        {
            var directory = Path.GetDirectoryName(path);
            if (!string.IsNullOrWhiteSpace(directory))
            {
                Directory.CreateDirectory(directory);
            }

            var json = JsonSerializer.Serialize(config, s_jsonSerializerOptions);
            File.WriteAllText(path, json);
        }
    }
}
