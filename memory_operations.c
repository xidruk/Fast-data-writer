#include "boosting_package.h"

void memory_copy(char *dst, char *src)
{
    int _Idx = 0;
    if (!dst || !src)
        return ;
    while (src[_Idx])
    {
        dst[_Idx] = src[_Idx];
        _Idx++;
    }
    dst[_Idx] = '\0';
}

char *data_dup(const char *data)
{
    int _Idx = 0;

    if (!data)
        return (NULL);
    int data_size =  get_string_len(data);
    char *new_data = malloc(data_size + 1);
    if (!new_data)
        return (NULL);
    for (_Idx = 0; _Idx < data_size; _Idx++)
    {
        new_data[_Idx] = data[_Idx];
    }
    new_data[data_size] = '\0';
    return (new_data);
}

char *expand_data(const char *old_data, const char *new_data)
{
    int _Idx = 0;

    if (!old_data || !new_data)
        return (NULL);
    int old_data_size = get_string_len(old_data);
    int new_data_size = get_string_len(new_data);

    char *expanded_data;
    expanded_data = malloc(old_data_size + new_data_size + 1);
    if (!expanded_data)
        return (NULL);
    int tracker = 0;
    while (old_data[_Idx])
    {
        expanded_data[_Idx] = old_data[_Idx];
        _Idx++;
    }
    tracker = _Idx;
    _Idx = 0;
    while (new_data[_Idx])
        expanded_data[tracker++] = new_data[_Idx++];
    expanded_data[old_data_size + new_data_size] = '\0';
    return (expanded_data);    
}

