/*
 * Developed by: Diego Hommerding Amorim - 00341793
 *           Gabriel Kenji Yatsuda Ikuta - 00337491
 */

#include <stdio.h>
#include <string.h>

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

    // Default output mode
    enum { OUTPUT_ASM, OUTPUT_ILOC } output_mode = OUTPUT_ASM;

    // Check for CLI argument
    if (argc > 1) {
        if (strcmp(argv[1], "--iloc") == 0) {
            output_mode = OUTPUT_ILOC;
        } else if (strcmp(argv[1], "--asm") == 0) {
            output_mode = OUTPUT_ASM;
        } else {
            fprintf(stderr, "Usage: %s [--asm|--iloc]\n", argv[0]);
            return 1;
        }
    }

    int ret = yyparse();
    iloc_op_list_t *code = (tree != NULL) ? tree->code : NULL;

    if (output_mode == OUTPUT_ILOC) {
        print_iloc_list(code);
    } else {
        print_asm(scope_stack->tables[0], code);
    }

    if (tree) {
        iloc_op_list_free(tree->code);
        asd_free(tree);
    }
    scope_stack_free(scope_stack);
    yylex_destroy();
    return ret;
}
