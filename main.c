/*
Função principal para realização da análise sintática e criação da AST.
Desenvolvido por:
  Professor Lucas Mello Schnorr
Usado por:
  Diego Hommerding Amorim - 00341793
  Gabriel Gabriel Kenji Yatsuda Ikuta - 00337491
*/

#include <stdio.h>

#include "asd.h"
extern int yyparse(void);
extern int yylex_destroy(void);
asd_tree_t *tree = NULL;
int main(int argc, char **argv)
{
    int ret = yyparse();
    asd_print_graphviz(tree);
    asd_free(tree);
    yylex_destroy();
    return ret;
}
