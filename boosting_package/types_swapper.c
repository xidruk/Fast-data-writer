#include "boosting_package.h"

char *int_to_str(int num)
{
    int is_negative = 0;
    unsigned int abs_value;
    int len;
    char *result;
    int pos;
    
    if (num < 0) {
        is_negative = 1;
        abs_value = (unsigned int)(-num);
    } else {
        abs_value = (unsigned int)num;
    }
    
    //---> calculate length of the given integer
    if (abs_value == 0) {
        len = 1;
    } else {
        len = 0;
        unsigned int temp = abs_value;
        while (temp > 0) {
            temp /= 10;
            len++;
        }
    }
    
    //---> allocate memory for the result
    result = malloc(len + is_negative + 1);
    if (!result) return NULL;
    
    //---> fill in the string
    pos = len + is_negative - 1;
    result[len + is_negative] = '\0';
    
    if (abs_value == 0) {
        result[0] = '0';
    } else {
        while (abs_value > 0) {
            result[pos--] = '0' + (abs_value % 10);
            abs_value /= 10;
        }
    }
    
    if (is_negative) {
        result[0] = '-';
    }
    
    return result;
}

