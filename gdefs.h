#ifndef GDEFS
#define GDEFS

//---> includes 
#include "./gheader.h"

// format with $ 

typedef struct fprinter
{
    int _fd; // file descriptor to write on when the file dosent exist  , pass -1 negative for nofound value
    unsigned char *_filepath; // to write to a fileb pass null for nofile
    unsigned char * buffer; // pointer to the buffer data you want to print 
    void **_p_data; // data you want to print end with a nulll
    int _flags; // flags for what you want 
    int _error_stat; // error states 
} _f_printer;

// Define error states
#define FP_ERROR_NONE 0
#define FP_ERROR_FILE_ACCESS 1
#define FP_ERROR_MEMORY 2
#define FP_ERROR_INVALID_ARGS 3

// Define flags
#define FP_FLAG_NONE 0
#define FP_FLAG_APPEND 1


#endif