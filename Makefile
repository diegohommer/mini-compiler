# Developed by: Diego Hommerding Amorim - 00341793
#               Gabriel Kenji Yatsuda Ikuta - 00337491

# Compiler and flags
CC = gcc
FLEX = flex
BISON = bison
CFLAGS = -Wall -Werror -fsanitize=address

# Files
_DEPS = parser.tab.h asd.h symbol_table.h scope_stack.h type_infer.h errors.h
DEPS = $(_DEPS)

# Source files (including generated ones) all in root
SRC := $(shell find . -maxdepth 1 -name '*.c') \
       ./parser.tab.c \
       ./lex.yy.c

OBJ := $(patsubst %.c,%.o,$(SRC))

EXEC = etapa4.out

# Default execution to all rules
all: $(EXEC)

# No directories to create

# Rule to compile the .y bison file (output in root)
parser.tab.h parser.tab.c: parser.y
	$(BISON) -d -W --header=parser.tab.h -o parser.tab.c $<

# Rule to compile the .l flex file (output in root)
lex.yy.c: scanner.l
	$(FLEX) -o lex.yy.c scanner.l

# Rule to compile .c files into .o files (all in root)
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Rule to link object files into the final executable
$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o *.out parser.tab.h parser.tab.c lex.yy.c
