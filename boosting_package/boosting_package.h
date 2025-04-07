#ifndef BOOSTING_PACKAGE
#define BOOSTING_PACKAGE

#include <unistd.h>
#include <stdlib.h>

//--->functions 
ssize_t get_string_len(const char *str);
ssize_t _get_decimal_len(int num);
void memory_copy(char *dst, char *src);
char *data_dup(const char *data);
char *expand_data(const char *old_data, const char *new_data);
char *int_to_str(int num);

#endif
