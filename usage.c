#include "gheader.h"
#include "gdefs.h"

int main(void) {
    // Initialize the printer
    f_printer *printer = init_fprinter();
    
    // Set the output buffer with a format string
    // Format specifiers: $s for string, $d for decimal, $c for character
    fprinter_set_buffer(printer, "Character: $c, String: $s, Decimal: $d\n");
    
    // Prepare data to be printed
    char ch = 'A';
    char *str = "fwriter";
    int number = 123;
    void *data[] = { &ch, str, &number, NULL };
    fprinter_set_data(printer, data);
    
    // Perform the printing operation
    fast_printer(printer);
    
    // Clean up resources
    free_fprinter(printer);
    
    return 0;
}
