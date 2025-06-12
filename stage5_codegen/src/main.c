/*
 * Developed by: Diego Hommerding Amorim - 00341793
 *           Gabriel Kenji Yatsuda Ikuta - 00337491
 */

#include "iloc_gen.h"
#include "scope_stack.h"
extern int yyparse(void);
extern int yylex_destroy(void);
asd_tree_t *tree = NULL;
scope_stack_t *scope_stack = NULL;

int main(int argc, char **argv)
{
    scope_stack = scope_stack_new();
    int ret = yyparse();
    asd_print_graphviz(tree);
    iloc_op_list_free(tree->code);
    asd_free(tree);
    scope_stack_free(scope_stack);
    yylex_destroy();
    return ret;
}
