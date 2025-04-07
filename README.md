# fast data writer

fast data writer is a high-performance printing library written in C with inline assembly optimizations. Designed for speed and flexibility, fwriter supports printing of characters, strings, and decimal numbers, while offering multiple formatting options through a custom format specifier syntax.

## Description

At the core of fwriter is a custom structure that encapsulates all printing parameters and resources. The structure, defined as `f_printer`, is designed to manage the output destination, buffer, and dynamic data to be printed. Its key components are:

- **File Descriptor (`_fd`):**  
  Determines the output destination. By default, it is set to standard output (`STDOUT_FILENO`), but can be updated to a file descriptor if output to a file is desired. A negative value (e.g., -1) indicates an invalid descriptor.

- **File Path Pointer (`_filepath`):**  
  Points to the file path where output should be directed. Passing `NULL` means output will not be written to a file, allowing fwriter to fall back on standard output.

- **Buffer (`buffer`):**  
  Holds the string data that will be processed and printed. The library duplicates and processes this buffer to handle format specifiers.

- **Data Pointer Array (`_p_data`):**  
  An array of pointers holding the data values (e.g., strings, integers, characters) to be formatted into the buffer. The array must be terminated with a `NULL` pointer.

- **Flags (`_flags`):**  
  Control various printing options such as appending to an existing file or other user-defined behaviors.

- **Error Status (`_error_stat`):**  
  Records error states during operations, allowing the calling code to handle issues such as memory allocation failures or file access errors.

The library integrates a fast write mechanism using inline assembly for the actual output operation. This assembly routine leverages the system call interface to minimize overhead, providing a significant speed boost when compared to standard C library functions.

In addition to the core printing functionality, the project includes a *boosting package* that provides utility functions for:
- Determining string lengths.
- Copying and duplicating memory blocks.
- Converting integers to strings.
- Expanding and concatenating strings.

These utilities support the formatting functions in `fast_printer.c`, ensuring that format specifiers such as `$s` (string), `$d` (decimal), and `$c` (character) are processed accurately.

## Features

- **Fast Output:**  
  Utilizes inline assembly for system-level write operations, achieving minimal overhead.

- **Flexible Formatting:**  
  Supports custom format specifiers to inject strings, integers, and characters into output buffers dynamically.

- **Dynamic Resource Management:**  
  The `f_printer` structure encapsulates file, buffer, and dynamic data management for streamlined printing.

- **Utility Functions:**  
  A dedicated boosting package provides helper functions for common string and memory operations.

## Repository Structure

- **Top-Level Files:**
  - `fprinter_utils.c`: Contains helper functions to initialize the printer and set up buffers and file paths.
  - `fast_printer.c`: Implements the core printing function, processing format specifiers and executing the optimized write.
  - `gheader.h`, `gdefs.h`: Header files with declarations for the printer structure, error codes, flags, and function prototypes.

- **Boosting Package (Directory: `boosting_package/`):**
  - `boosting_package.h`: Header file for the utility functions.
  - `get_sizes.c`, `memory_operations.c`, `types_swapper.c`: Implementation files for string length determination, memory operations, and type conversions.

## Build Instructions

A provided Makefile compiles all source files, storing object files in the `obj/` directory and the resulting static library in the `lib/` directory. To build the project, simply run:

```bash
make
