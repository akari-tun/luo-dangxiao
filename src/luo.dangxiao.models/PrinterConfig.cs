using System;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace luo.dangxiao.models
{
    /// <summary>
    /// Warning metadata produced while resolving printer provider configuration.
    /// </summary>
    public sealed class PrinterProviderResolutionWarning
    {
        /// <summary>
        /// Gets or sets the configured provider value that could not be used.
        /// </summary>
        public string InvalidProviderValue { get; set; } = string.Empty;

        /// <summary>
        /// Gets or sets the provider selected as fallback.
        /// </summary>
        public PrinterProvider ResolvedProvider { get; set; }
    }

    /// <summary>
    /// Supported card printer providers.
    /// </summary>
    public enum PrinterProvider
    {
        Unknown,
        Seaory,
        Virtual
    }

    /// <summary>
    /// JSON converter for printer providers with graceful fallback for invalid values.
    /// </summary>
    public sealed class PrinterProviderJsonConverter : JsonConverter<PrinterProvider>
    {
        /// <summary>
        /// Gets the last raw provider value that failed to parse.
        /// </summary>
        public static string? LastInvalidValue { get; private set; }

        /// <inheritdoc />
        public override PrinterProvider Read(ref Utf8JsonReader reader, Type typeToConvert, JsonSerializerOptions options)
        {
            if (reader.TokenType == JsonTokenType.String)
            {
                var value = reader.GetString();
                if (Enum.TryParse<PrinterProvider>(value, ignoreCase: true, out var provider))
                {
                    LastInvalidValue = null;
                    return provider;
                }

                LastInvalidValue = value ?? string.Empty;
                return PrinterProvider.Unknown;
            }

            if (reader.TokenType == JsonTokenType.Number && reader.TryGetInt32(out var number))
            {
                if (Enum.IsDefined(typeof(PrinterProvider), number))
                {
                    LastInvalidValue = null;
                    return (PrinterProvider)number;
                }

                LastInvalidValue = number.ToString();
                return PrinterProvider.Unknown;
            }

            LastInvalidValue = reader.TokenType.ToString();
            return PrinterProvider.Unknown;
        }

        /// <inheritdoc />
        public override void Write(Utf8JsonWriter writer, PrinterProvider value, JsonSerializerOptions options)
        {
            writer.WriteStringValue(value.ToString());
        }
    }

    /// <summary>
    /// Configuration for selecting and initializing a card printer provider.
    /// </summary>
    public sealed class PrinterConfig
    {
        /// <summary>
        /// Gets or sets the printer provider type.
        /// </summary>
        [JsonConverter(typeof(PrinterProviderJsonConverter))]
        public PrinterProvider Provider { get; set; } = PrinterProvider.Seaory;

        /// <summary>
        /// Gets or sets the raw provider value captured during configuration parsing.
        /// </summary>
        public string RawProviderValue { get; set; } = string.Empty;

        /// <summary>
        /// Gets or sets the default printer identifier.
        /// </summary>
        public string DefaultPrinterId { get; set; } = string.Empty;

        /// <summary>
        /// Resolves the configured provider to a supported runtime provider.
        /// </summary>
        /// <param name="warning">A warning payload when fallback is applied.</param>
        /// <returns>The resolved provider value.</returns>
        public PrinterProvider ResolveProvider(out PrinterProviderResolutionWarning? warning)
        {
            if (Provider != PrinterProvider.Unknown)
            {
                warning = null;
                return Provider;
            }

            warning = new PrinterProviderResolutionWarning
            {
                InvalidProviderValue = string.IsNullOrWhiteSpace(RawProviderValue)
                    ? PrinterProviderJsonConverter.LastInvalidValue ?? string.Empty
                    : RawProviderValue,
                ResolvedProvider = PrinterProvider.Seaory
            };

            Provider = PrinterProvider.Seaory;
            return Provider;
        }
    }
}
