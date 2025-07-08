# Directories
IDIR = include
SDIR = src
ODIR = obj
BDIR = bin


# Compiler and flags
CC = gcc
FLEX = flex
BISON = bison
CFLAGS = -I$(IDIR) -Wall -Werror -fsanitize=address


# Files
_DEPS = parser.tab.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

# Source files (including generated ones)
SRC := $(shell find $(SDIR) -name '*.c') \
       $(SDIR)/syntax/parser.tab.c \
       $(SDIR)/lexical/lex.yy.c
 
OBJ := $(patsubst $(SDIR)/%.c,$(ODIR)/%.o,$(SRC))

EXEC = $(BDIR)/mini_compiler

# Default execution to all rules
all: $(EXEC)

# Rule to build include directory if it doesn't exist
$(IDIR):
	mkdir -p $(IDIR)

# Rule to build bin directory if it doesn't exist
$(BDIR):
	mkdir -p $(BDIR)

# Rule to build obj directory if it doesn't exist
$(ODIR):
	mkdir -p $(ODIR)

# Rule to compile the .y bison file
$(IDIR)/parser.tab.h $(SDIR)/syntax/parser.tab.c: $(SDIR)/syntax/parser.y | $(IDIR) $(ODIR)
	$(BISON) -d -W --header=$(IDIR)/parser.tab.h -o $(SDIR)/syntax/parser.tab.c $<

# Rule to compile the .l flex file
$(SDIR)/lexical/lex.yy.c: $(SDIR)/lexical/scanner.l
	$(FLEX) -o $@ $<

# Rule to compile .c files into .o files
$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	@mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

# Rule to link object files into the final executable
$(EXEC): $(OBJ) | $(BDIR)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf $(ODIR) $(BDIR)
	rm -f $(IDIR)/parser.tab.h $(SDIR)/lexical/lex.yy.c $(SDIR)/syntax/parser.tab.c
