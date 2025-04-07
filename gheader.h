#ifndef GHEAFER
# define GHEAFER

//---> includes 
//--> system include

# include <stdio.h>
# include <fcntl.h>
# include <syscall.h>
# include <sys/types.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
#include <errno.h>
#include "boosting_package/boosting_package.h"


//---> P-Functions 

typedef struct fprinter f_printer;

f_printer *init_fprinter(void);
ssize_t fast_printer(f_printer *fprinter);
void free_fprinter(f_printer *fprinter);

// Helper functions for user API
int fprinter_set_buffer(f_printer *fprinter, const char *format);
int fprinter_set_file(f_printer *fprinter, const char *filepath, int flags);
int fprinter_set_data(f_printer *fprinter, void **data);



#endif