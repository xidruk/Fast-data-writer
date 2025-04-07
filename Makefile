# Compiler and flags
CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -I.
VPATH   = . boosting_package

# Directories
OBJDIR  = obj
LIBDIR  = lib

# Source files (top-level and boosting_package)
SRC_TOP   = fprinter_utils.c fast_printer.c 
SRC_BOOST = boosting_package/get_sizes.c boosting_package/memory_operations.c boosting_package/types_swapper.c
SRCS      = $(SRC_TOP) $(SRC_BOOST)

# Object files: convert each source into an object file in the obj directory.
# This uses only the basename (make sure no files share the same name).
OBJS      = $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(SRCS)))

# Name of the static library in the lib directory
LIB       = $(LIBDIR)/libcprinter.a

.PHONY: all clean dirs

all: dirs $(LIB)

# Create required directories if they don't exist.
dirs:
	mkdir -p $(OBJDIR) $(LIBDIR)

# Archive the object files into a static library.
$(LIB): $(OBJS)
	@echo "Archiving object files into $(LIB)..."
	ar rcs $(LIB) $(OBJS)
	@echo "Static library $(LIB) created."

# Rule to compile any .c file (found via VPATH) into the corresponding object file in obj/
$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts.
clean:
	@echo "Cleaning up..."
	rm -rf $(OBJDIR) $(LIB)
	@echo "Clean complete."
