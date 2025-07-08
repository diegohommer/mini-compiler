/*
 * Developed by: Diego Hommerding Amorim - 00341793
 *           Gabriel Kenji Yatsuda Ikuta - 00337491
 */

#include "asm_gen.h"
#include "iloc_ir.h"
#include "scope_stack.h"
extern int yyparse(void);
extern int yylex_destroy(void);
asd_tree_t *tree = NULL;
scope_stack_t *scope_stack = NULL;

int main(int argc, char **argv)
{
    scope_stack = scope_stack_new();
    int ret = yyparse();
    if (tree) {
        print_asm(scope_stack->tables[0], tree->code);
        iloc_op_list_free(tree->code);
        asd_free(tree);
    }
    scope_stack_free(scope_stack);
    yylex_destroy();
    return ret;
}
