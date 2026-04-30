using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.DependencyInjection;
using luo.dangxiao.resources.Languages;
using luo.dangxiao.wabapi.Clients;
using luo.dangxiao.wabapi.Dtos.Responses;
using System.Text.Json;

namespace luo.dangxiao.cardcenter.ViewModels
{
    /// <summary>
    /// Base class for all ViewModels providing shared YktApi calling helpers.
    /// </summary>
    public abstract partial class ViewModelBase : ObservableObject
    {
        #region YktApi Helpers

        /// <summary>
        /// Gets the configured <see cref="IYktApiClient"/> from the DI container.
        /// Returns null if the client is not registered.
        /// </summary>
        protected static IYktApiClient? GetYktApiClient()
        {
            return Ioc.Default.GetService<IYktApiClient>();
        }

        /// <summary>
        /// Determines whether an API response code indicates success.
        /// </summary>
        protected static bool IsApiSuccess(int? code)
        {
            return code is null or 0 or 200;
        }

        /// <summary>
        /// Determines whether an API response indicates success using both <see cref="Success"/> and <see cref="Code"/>.
        /// </summary>
        protected static bool IsApiSuccess<TData>(ApiResponseDto<TData> response)
        {
            return response.Success == true || IsApiSuccess(response.Code);
        }

        /// <summary>
        /// Validates an API response and throws <see cref="InvalidOperationException"/> on failure.
        /// </summary>
        protected static void EnsureApiSuccess(int? code, string? message, string? fallbackResourceKey = null)
        {
            if (IsApiSuccess(code))
            {
                return;
            }

            var errorMsg = string.IsNullOrWhiteSpace(message)
                ? LanguageProvider.GetLocalizedText(fallbackResourceKey ?? "Msg_Error")
                : message;
            throw new InvalidOperationException(errorMsg);
        }

        /// <summary>
        /// Validates an API response and throws <see cref="InvalidOperationException"/> on failure.
        /// </summary>
        protected static void EnsureApiSuccess<TData>(ApiResponseDto<TData> response, string? fallbackResourceKey = null)
        {
            if (IsApiSuccess(response))
            {
                return;
            }

            var errorMsg = string.IsNullOrWhiteSpace(response.Message)
                ? LanguageProvider.GetLocalizedText(fallbackResourceKey ?? "Msg_Error")
                : response.Message;
            throw new InvalidOperationException(errorMsg);
        }

        /// <summary>
        /// Extracts a string-typed property value from a <see cref="JsonElement"/> by trying field names in order.
        /// </summary>
        protected static string? ExtractJsonString(JsonElement? data, params string[] fieldNames)
        {
            if (!data.HasValue || data.Value.ValueKind != JsonValueKind.Object)
            {
                return null;
            }

            foreach (var name in fieldNames)
            {
                if (data.Value.TryGetProperty(name, out var prop) && prop.ValueKind == JsonValueKind.String)
                {
                    return prop.GetString();
                }
            }

            return null;
        }

        /// <summary>
        /// Formats an API error message for user-friendly UI display.
        /// </summary>
        protected static string FormatApiError(string? apiMessage, string localizedFallback)
        {
            return string.IsNullOrWhiteSpace(apiMessage)
                ? localizedFallback
                : apiMessage;
        }

        #endregion
    }
}
