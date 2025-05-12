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
_DEPS = parser.tab.h asd.h table.h errors.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o lex.yy.o parser.tab.o asd.o table.o errors.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

EXEC = $(BDIR)/stage4.out

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
$(IDIR)/parser.tab.h $(SDIR)/parser.tab.c: $(SDIR)/parser.y | $(IDIR) $(ODIR)
	$(BISON) -d -W --header=$(IDIR)/parser.tab.h -o $(SDIR)/parser.tab.c $<

# Rule to compile the .l flex file
$(SDIR)/lex.yy.c: $(SDIR)/scanner.l
	$(FLEX) -o $@ $<

# Rule to compile .c files into .o files
$(ODIR)/%.o: $(SDIR)/%.c $(DEPS) | $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

# Rule to link object files into the final executable
$(EXEC): $(OBJ) | $(BDIR)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(ODIR)/*.o $(EXEC) $(IDIR)/parser.tab.h $(SDIR)/lex.yy.c $(SDIR)/parser.tab.c