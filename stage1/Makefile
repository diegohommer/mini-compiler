# Directories
IDIR = include
SDIR = src
ODIR = obj
BDIR = bin


# Compiler and flags
CC = gcc
FLEX = flex
CFLAGS = -I$(IDIR)


# Files
_DEPS = tokens.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o lex.yy.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

EXEC = $(BDIR)/main.out


# Default execution to all rules
all: $(EXEC)

# Rule to compile the .l flex file
$(SDIR)/lex.yy.c: $(SDIR)/scanner.l
	$(FLEX) -o $@ $<

# Rule to compile .c files into .o files
$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Rule to link object files into the final executable
$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(ODIR)/*.o $(EXEC)