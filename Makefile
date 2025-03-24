# Directories
IDIR = .
SDIR = .
ODIR = ./obj


# Compiler and flags
CC = gcc
FLEX = flex
CFLAGS = -I$(IDIR)


# Files
_DEPS = tokens.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o lex.yy.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

EXEC = etapa1

# Default execution to all rules
all: $(EXEC)

# Rule to build obj directory if it doesn't exist
$(ODIR):
	mkdir -p $(ODIR)

# Rule to compile the .l flex file
$(SDIR)/lex.yy.c: $(SDIR)/scanner.l
	$(FLEX) -o $@ $<

# Rule to compile .c files into .o files
$(ODIR)/%.o: $(SDIR)/%.c $(DEPS) | $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

# Rule to link object files into the final executable
$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(ODIR)/*.o $(EXEC)