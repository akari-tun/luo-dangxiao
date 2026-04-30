using System;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace luo.dangxiao.models
{
    public sealed class ReaderProviderResolutionWarning
    {
        public string InvalidProviderValue { get; set; } = string.Empty;

        public ReaderProvider ResolvedProvider { get; set; }
    }

    public enum ReaderProvider
    {
        Unknown,
        Yc,
        Virtual
    }

    public sealed class ReaderProviderJsonConverter : JsonConverter<ReaderProvider>
    {
        public static string? LastInvalidValue { get; private set; }

        public override ReaderProvider Read(ref Utf8JsonReader reader, Type typeToConvert, JsonSerializerOptions options)
        {
            if (reader.TokenType == JsonTokenType.String)
            {
                var value = reader.GetString();
                if (Enum.TryParse<ReaderProvider>(value, ignoreCase: true, out var provider))
                {
                    LastInvalidValue = null;
                    return provider;
                }

                LastInvalidValue = value ?? string.Empty;
                return ReaderProvider.Unknown;
            }

            if (reader.TokenType == JsonTokenType.Number && reader.TryGetInt32(out var number))
            {
                if (Enum.IsDefined(typeof(ReaderProvider), number))
                {
                    LastInvalidValue = null;
                    return (ReaderProvider)number;
                }

                LastInvalidValue = number.ToString();
                return ReaderProvider.Unknown;
            }

            LastInvalidValue = reader.TokenType.ToString();
            return ReaderProvider.Unknown;
        }

        public override void Write(Utf8JsonWriter writer, ReaderProvider value, JsonSerializerOptions options)
        {
            writer.WriteStringValue(value.ToString());
        }
    }

    public sealed class ReaderConfig
    {
        [JsonConverter(typeof(ReaderProviderJsonConverter))]
        public ReaderProvider Provider { get; set; } = ReaderProvider.Virtual;

        public string RawProviderValue { get; set; } = string.Empty;

        public ReaderProvider ResolveProvider(out ReaderProviderResolutionWarning? warning)
        {
            if (Provider != ReaderProvider.Unknown)
            {
                warning = null;
                return Provider;
            }

            warning = new ReaderProviderResolutionWarning
            {
                InvalidProviderValue = string.IsNullOrWhiteSpace(RawProviderValue)
                    ? ReaderProviderJsonConverter.LastInvalidValue ?? string.Empty
                    : RawProviderValue,
                ResolvedProvider = ReaderProvider.Yc
            };

            Provider = ReaderProvider.Yc;
            return Provider;
        }
    }
}
