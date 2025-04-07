// fast_printer.c
#include "gdefs.h"
#include "gheader.h"

// ?? function to initialize the output buffer pool
static unsigned char* init_output_pool(f_printer *fprinter)
{
    if (!fprinter || !fprinter->buffer)
        return NULL;
    
    return (unsigned char *)data_dup((const char *)fprinter->buffer);
}

// Function to replace a string format specifier
static int replace_string_format(unsigned char **buffer, unsigned char *format_pos, char *replacement) {
    if (!replacement) replacement = "(null)";
    
    // Calculate positions
    size_t prefix_len = format_pos - *buffer;
    size_t str_len = get_string_len(replacement);
    size_t suffix_len = get_string_len((char *)(format_pos + 2));
    
    // Allocate new buffer
    unsigned char *new_buffer = malloc(prefix_len + str_len + suffix_len + 1);
    if (!new_buffer) {
        return FP_ERROR_MEMORY;
    }
    
    // Copy prefix
    memcpy(new_buffer, *buffer, prefix_len);
    
    // Copy replacement string
    memcpy(new_buffer + prefix_len, replacement, str_len);
    
    // Copy suffix
    memcpy(new_buffer + prefix_len + str_len, format_pos + 2, suffix_len + 1);
    
    // Update buffer
    free(*buffer);
    *buffer = new_buffer;
    
    return FP_ERROR_NONE;
}

// Function to replace an integer format specifier
static int replace_int_format(unsigned char **buffer, unsigned char *format_pos, int value) {
    char *str_value = int_to_str(value);
    
    if (!str_value) {
        return FP_ERROR_MEMORY;
    }
    
    int result = replace_string_format(buffer, format_pos, str_value);
    free(str_value);
    
    return result;
}

static int replace_char_format(unsigned char **buffer, unsigned char *format_pos, char value) {
    char char_str[2] = {value, '\0'};
    return replace_string_format(buffer, format_pos, char_str);
}

// Function to process all format specifiers in the buffer
static int process_format_specifiers(f_printer *fprinter, unsigned char **output_pool)
{
    int data_idx = 0;
    int error = FP_ERROR_NONE;
    
    while (1) {
        // Find the next format specifier
        unsigned char *format_pos = (unsigned char *)strchr((char *)*output_pool, '$');
        if (!format_pos || format_pos[1] == '\0') 
            break; // No more format specifiers or $ at the end
        
        // Check if we have data to process
        if (!fprinter->_p_data || !fprinter->_p_data[data_idx]) {
            break;
        }
        
        // Handle different format specifiers
        switch (format_pos[1]) {
            case 's': {
                // Replace $s with string
                char *str_value = (char *)fprinter->_p_data[data_idx];
                error = replace_string_format(output_pool, format_pos, str_value);
                break;
            }
            case 'd': {
                // Replace $d with decimal number
                int int_value = *((int *)fprinter->_p_data[data_idx]);
                error = replace_int_format(output_pool, format_pos, int_value);
                break;
            }
            case 'c': {
                // Replace $c with character
                char char_value = *((char *)fprinter->_p_data[data_idx]);
                error = replace_char_format(output_pool, format_pos, char_value);
                break;
            }
            default:
                // Skip unknown format specifier
                *output_pool = (unsigned char *)strcpy((char *)*output_pool, (char *)*output_pool + 1);
                continue; // Don't increment data_idx for unknown format
        }
        
        if (error != FP_ERROR_NONE) 
            return error;
        
        data_idx++;
    }
    
    return FP_ERROR_NONE;
}

// Function to handle file opening
static int prepare_output_file(f_printer *fprinter)
{
    if (!fprinter->_filepath || fprinter->_fd > 0)
        return FP_ERROR_NONE;
    
    // Check if file is accessible
    if (access((const char *)fprinter->_filepath, F_OK) != 0) {
        // Create the file if it doesn't exist
        fprinter->_fd = open((const char *)fprinter->_filepath, 
                           O_CREAT | O_WRONLY | O_TRUNC, 0644);
    } else {
        // File exists, open with appropriate flags
        int flags = O_WRONLY;
        if (fprinter->_flags & FP_FLAG_APPEND)
            flags |= O_APPEND;
        else
            flags |= O_TRUNC;
            
        fprinter->_fd = open((const char *)fprinter->_filepath, flags, 0644);
    }
    
    if (fprinter->_fd < 0) {
        return FP_ERROR_FILE_ACCESS;
    }
    
    return FP_ERROR_NONE;
}

// Function to perform the actual write operation
#include <errno.h>  // For errno handling

static ssize_t write_output(f_printer *fprinter, unsigned char *output_pool)
{
    if (!output_pool) return -1;
    
    size_t len = get_string_len((char *)output_pool);
    int fd = fprinter->_fd;
    ssize_t ret;

    __asm__ __volatile__ (
        "syscall"
        : "=a" (ret)
        : "a" (1),           //---> SYS_write = 1 on x86-64
          "D" (fd),          //---> file descriptor (1st argument)
          "S" (output_pool), //---> buffer pointer (2nd argument)
          "d" (len)          //---> length (3rd argument)
        : "rcx", "r11",      //---> clobbered registers
          "memory"           //---> memory might be affected
    );

    //---> handle error (syscall returns negative errno codes)
    if (ret < 0)
    {
        errno = -ret;  // convert to positive errno value
        return -1;
    }
    return ret;
}

//  main function 
ssize_t fast_printer(f_printer *fprinter)
{
    if (!fprinter || !fprinter->buffer)
        return -1;
    
    ssize_t written = -1;
    unsigned char *output_pool = NULL;
    
    //---> initialize the output pool
    output_pool = init_output_pool(fprinter);
    if (!output_pool)
    {
        fprinter->_error_stat = FP_ERROR_MEMORY;
        return -1;
    }
    
    //---> process all format specifiers
    fprinter->_error_stat = process_format_specifiers(fprinter, &output_pool);
    if (fprinter->_error_stat != FP_ERROR_NONE)
    {
        free(output_pool);
        return -1;
    }
    
    //---> prepare the output file if needed
    if (fprinter->_filepath != NULL)
    {
        fprinter->_error_stat = prepare_output_file(fprinter);
        if (fprinter->_error_stat != FP_ERROR_NONE)
        {
            free(output_pool);
            return -1;
        }
    }
    
    // Step 4: Write the formatted output
    written = write_output(fprinter, output_pool);
    
    //---> clean up the pool
    free(output_pool);
    return written;
}

// Function to free the printer and its resources
void free_fprinter(f_printer *fprinter) {
    if (!fprinter) return;
    
    if (fprinter->buffer) free(fprinter->buffer);
    if (fprinter->_filepath) free(fprinter->_filepath);
    
    // Close file descriptor if it's valid and we opened it
    if (fprinter->_fd > 0 && fprinter->_fd != STDOUT_FILENO) 
        close(fprinter->_fd);
    
    free(fprinter);
}
