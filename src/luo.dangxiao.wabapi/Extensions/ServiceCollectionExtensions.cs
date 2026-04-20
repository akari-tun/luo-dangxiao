using luo.dangxiao.wabapi.Clients;
using luo.dangxiao.wabapi.Options;
using Microsoft.Extensions.DependencyInjection;

namespace luo.dangxiao.wabapi.Extensions
{
    public static class ServiceCollectionExtensions
    {
        public static IServiceCollection AddYktWabApi(this IServiceCollection services, string baseUrl, int timeoutSeconds = 30)
        {
            return services.AddYktWabApi(new YktApiOptions
            {
                BaseUrl = baseUrl,
                TimeoutSeconds = timeoutSeconds
            });
        }

        public static IServiceCollection AddYktWabApi(this IServiceCollection services, YktApiOptions options)
        {
            ArgumentNullException.ThrowIfNull(services);
            ArgumentNullException.ThrowIfNull(options);

            if (string.IsNullOrWhiteSpace(options.BaseUrl))
            {
                throw new ArgumentException("BaseUrl can not be empty.", nameof(options));
            }

            if (!Uri.TryCreate(options.BaseUrl, UriKind.Absolute, out Uri? baseAddress))
            {
                throw new ArgumentException("BaseUrl is invalid.", nameof(options));
            }

            services.AddSingleton(options);
            services.AddSingleton<IYktApiClient>(_ =>
            {
                var httpClient = new HttpClient
                {
                    BaseAddress = baseAddress,
                    Timeout = TimeSpan.FromSeconds(options.TimeoutSeconds)
                };

                return new YktApiClient(httpClient);
            });

            return services;
        }
    }
}
