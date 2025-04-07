# Definição do Makefile para compilação do projeto.
# Desenvolvido por:
#   Diego Hommerding Amorim - 00341793
#   Gabriel Gabriel Kenji Yatsuda Ikuta - 00337491

# Compiler and flags
CC = gcc
FLEX = flex
BISON = bison
CFLAGS = -Wall -Werror

# Files
_DEPS = parser.tab.h
DEPS = $(_DEPS)

_OBJ = main.o lex.yy.o parser.tab.o
OBJ = $(_OBJ)

EXEC = etapa2

# Default execution to all rules
all: $(EXEC)

# Rule to compile the .y bison file
parser.tab.h parser.tab.c: parser.y
	$(BISON) -d --header=parser.tab.h -o parser.tab.c $<

# Rule to compile the .l flex file
lex.yy.c: scanner.l
	$(FLEX) -o $@ $<

# Rule to compile .c files into .o files
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Rule to link object files into the final executable
$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o parser.tab.* lex.yy.c $(EXEC)
