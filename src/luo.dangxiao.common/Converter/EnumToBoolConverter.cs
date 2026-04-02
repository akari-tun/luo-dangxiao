using Avalonia.Data.Converters;
using System;
using System.Globalization;

namespace luo.dangxiao.common.Converter;

/// <summary>
/// Converts enum value to boolean based on parameter
/// </summary>
public class EnumToBoolConverter : IValueConverter
{
    public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        if (value == null || parameter == null)
            return false;

        var enumValue = value.ToString();
        var parameterValue = parameter.ToString();

        return enumValue == parameterValue;
    }

    public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        throw new NotImplementedException();
    }
}
