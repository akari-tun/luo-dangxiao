using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.DependencyInjection;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.interfaces.ViewModels;
using luo.dangxiao.log;
using luo.dangxiao.resources.Languages;
using luo.dangxiao.wabapi.Clients;
using luo.dangxiao.wabapi.Dtos.Responses;
using System.Text.Json;

namespace luo.dangxiao.selfservice.ViewModels
{
    /// <summary>
    /// Base class for all ViewModels providing shared navigation and YktApi calling helpers.
    /// </summary>
    public abstract partial class ViewModelBase : ObservableObject, IPageViewModel
    {
        #region Logging

        /// <summary>
        /// Gets or sets the NLog logger instance for this ViewModel.
        /// Lazily resolved from DI container on first access.
        /// </summary>
        public NLog.ILogger Logger { get; set; } = null!;

        /// <summary>
        /// Resolves the logger for this instance type if not already set.
        /// </summary>
        protected void EnsureLogger()
        {
            if (Logger == null)
            {
                Logger = LoggerResolver.GetLogger(GetType());
            }
        }

        #endregion

        #region Navigation

        /// <summary>
        /// Navigates back to the home page.
        /// </summary>
        [RelayCommand]
        protected virtual void Back()
        {
            Ioc.Default.GetRequiredService<HomePageViewModel>().ReturnHome();
        }

        #endregion

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
        /// A response is considered successful when:
        /// - <see cref="Success"/> is explicitly true, OR
        /// - <see cref="Code"/> is null, 0, or 200.
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
        /// Returns the localized fallback message if <paramref name="apiMessage"/> is empty.
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
