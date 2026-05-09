using Microsoft.Extensions.DependencyInjection;

namespace luo.dangxiao.log;

public static class ServiceCollectionExtensions
{
    public static IServiceCollection AddNLogLogging(this IServiceCollection services)
    {
        services.AddSingleton<NLog.LogFactory>(sp => NLog.LogManager.LogFactory);
        return services;
    }
}
