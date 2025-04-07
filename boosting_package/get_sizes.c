#include "boosting_package.h"

ssize_t get_string_len(const char *str)
{
    int _len = 0;

    if (!str)
        return (-1);
    while (str[_len])
    {
        _len++;
    }
    return (_len);
}

ssize_t _get_decimal_len(int num)
{
    int _len = 0;
    int sign = 0;

    if (num == 0)
        return (1);
    if (num < 0)
    {
        sign = 1;
        num = -num;
    }
    while (num >= 10)
    {
        num /= 10;
        _len++; 
    }
    return (_len + sign);
}

