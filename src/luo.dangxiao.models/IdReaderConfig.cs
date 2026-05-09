using System;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace luo.dangxiao.models;

/// <summary>
/// Describes how an invalid ID reader provider value was resolved.
/// </summary>
public sealed class IdReaderProviderResolutionWarning
{
    /// <summary>
    /// Gets or sets the invalid provider value that was encountered.
    /// </summary>
    public string InvalidProviderValue { get; set; } = string.Empty;

    /// <summary>
    /// Gets or sets the provider that was selected as a fallback.
    /// </summary>
    public IdReaderProvider ResolvedProvider { get; set; }
}

/// <summary>
/// Defines the supported ID reader providers.
/// </summary>
public enum IdReaderProvider
{
    /// <summary>
    /// An unknown or invalid provider value.
    /// </summary>
    Unknown,

    /// <summary>
    /// Huashi CVR-100U ID reader.
    /// </summary>
    HuashiCvr100U,

    /// <summary>
    /// Virtual ID reader provider.
    /// </summary>
    Virtual
}

/// <summary>
/// Converts <see cref="IdReaderProvider"/> values to and from JSON.
/// </summary>
public sealed class IdReaderProviderJsonConverter : JsonConverter<IdReaderProvider>
{
    /// <summary>
    /// Gets the most recent invalid provider value encountered during deserialization.
    /// </summary>
    public static string? LastInvalidValue { get; private set; }

    /// <summary>
    /// Reads the provider value from JSON.
    /// </summary>
    /// <param name="reader">The JSON reader.</param>
    /// <param name="typeToConvert">The target type.</param>
    /// <param name="options">Serializer options.</param>
    /// <returns>The deserialized provider value, or <see cref="IdReaderProvider.Unknown"/> when the value is invalid.</returns>
    public override IdReaderProvider Read(ref Utf8JsonReader reader, Type typeToConvert, JsonSerializerOptions options)
    {
        if (reader.TokenType == JsonTokenType.String)
        {
            var value = reader.GetString();
            if (Enum.TryParse<IdReaderProvider>(value, ignoreCase: true, out var provider))
            {
                LastInvalidValue = null;
                return provider;
            }

            LastInvalidValue = value ?? string.Empty;
            return IdReaderProvider.Unknown;
        }

        if (reader.TokenType == JsonTokenType.Number && reader.TryGetInt32(out var number))
        {
            if (Enum.IsDefined(typeof(IdReaderProvider), number))
            {
                LastInvalidValue = null;
                return (IdReaderProvider)number;
            }

            LastInvalidValue = number.ToString();
            return IdReaderProvider.Unknown;
        }

        LastInvalidValue = reader.TokenType.ToString();
        return IdReaderProvider.Unknown;
    }

    /// <summary>
    /// Writes the provider value to JSON.
    /// </summary>
    /// <param name="writer">The JSON writer.</param>
    /// <param name="value">The provider value.</param>
    /// <param name="options">Serializer options.</param>
    public override void Write(Utf8JsonWriter writer, IdReaderProvider value, JsonSerializerOptions options)
    {
        writer.WriteStringValue(value.ToString());
    }
}

/// <summary>
/// Represents the configuration for the ID reader.
/// </summary>
public sealed class IdReaderConfig
{
    /// <summary>
    /// Gets or sets the configured provider.
    /// </summary>
    [JsonConverter(typeof(IdReaderProviderJsonConverter))]
    public IdReaderProvider Provider { get; set; } = IdReaderProvider.Virtual;

    /// <summary>
    /// Gets or sets the raw provider value read from configuration.
    /// </summary>
    public string RawProviderValue { get; set; } = string.Empty;

    /// <summary>
    /// Resolves the configured provider and returns a fallback when the value is unknown.
    /// </summary>
    /// <param name="warning">Receives a warning when the configured provider is invalid.</param>
    /// <returns>The resolved provider.</returns>
    public IdReaderProvider ResolveProvider(out IdReaderProviderResolutionWarning? warning)
    {
        if (Provider != IdReaderProvider.Unknown)
        {
            warning = null;
            return Provider;
        }

        warning = new IdReaderProviderResolutionWarning
        {
            InvalidProviderValue = string.IsNullOrWhiteSpace(RawProviderValue)
                ? IdReaderProviderJsonConverter.LastInvalidValue ?? string.Empty
                : RawProviderValue,
            ResolvedProvider = IdReaderProvider.Virtual
        };

        Provider = IdReaderProvider.Virtual;
        return Provider;
    }
}
