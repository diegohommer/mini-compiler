/*
Função principal para realização da análise sintática.
Desenvolvido por:
  Diego Hommerding Amorim - 00341793
  Gabriel Gabriel Kenji Yatsuda Ikuta - 00337491
*/

#include <stdio.h>
#include "parser.tab.h" //arquivo gerado com bison -d parser.y
                        //inclua tal comando no teu workflow (Makefile)
extern int yylex_destroy(void);

int main (int argc, char **argv)
{
  int ret = yyparse();
  yylex_destroy();
  return ret;
}
