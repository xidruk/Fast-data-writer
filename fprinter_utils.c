// fprinter_utils.c
#include "gheader.h"
#include "gdefs.h"

f_printer *init_fprinter(void)
{
    f_printer *fprinter;
    fprinter = malloc(sizeof(f_printer));
    if (!fprinter)
        return NULL;
    
    // Initialize all fields to safe defaults
    fprinter->buffer = NULL;
    fprinter->_error_stat = FP_ERROR_NONE;
    fprinter->_fd = STDOUT_FILENO; // Default to standard output
    fprinter->_filepath = NULL;
    fprinter->_flags = FP_FLAG_NONE;
    fprinter->_p_data = NULL;
    
    return fprinter;
}

// Helper function to set buffer data
int set_buffer(f_printer *fprinter, const char *format)
{
    if (!fprinter || !format)
        return FP_ERROR_INVALID_ARGS;
    
    // Free old buffer if it exists
    if (fprinter->buffer)
        free(fprinter->buffer);
    
    // Duplicate the format string
    fprinter->buffer = (unsigned char *)data_dup(format);
    if (!fprinter->buffer)
        return FP_ERROR_MEMORY;
    
    return FP_ERROR_NONE;
}

// ?? helper function to set file path
int set_filepath(f_printer *fprinter, const char *filepath)
{
    if (!fprinter || !filepath)
        return FP_ERROR_INVALID_ARGS;
    
    // ?? free old filepath if it exists
    if (fprinter->_filepath)
        free(fprinter->_filepath);
    
    // ?? duplicate the filepath
    fprinter->_filepath = (unsigned char *)data_dup(filepath);
    if (!fprinter->_filepath)
        return FP_ERROR_MEMORY;
    
    // ?? reset file descriptor to force new file opening
    fprinter->_fd = -1;
    
    return FP_ERROR_NONE;
}

int fprinter_set_buffer(f_printer *fprinter, const char *format)
{
    return set_buffer(fprinter, format);
}

int fprinter_set_file(f_printer *fprinter, const char *format, int flags)
{
    int result = set_filepath(fprinter, format);
    if (result == FP_ERROR_NONE)
    {
        fprinter->_flags = flags;
    }
    return result;
}

int fprinter_set_data(f_printer *fprinter, void **data)
{
    if (!fprinter)
        return (FP_ERROR_INVALID_ARGS);
    fprinter->_p_data = data;
    return FP_ERROR_NONE;
}