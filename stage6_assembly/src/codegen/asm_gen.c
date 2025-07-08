/*
 * Developed by: Diego Hommerding Amorim - 00341793
 *           Gabriel Kenji Yatsuda Ikuta - 00337491
 */
#include "asm_gen.h"

#include "asd.h"
#include "symbol_table.h"

void print_asm_global_var(const char* name)
{
    printf("\t.globl %s\n", name);
    printf("\t.bss\n");
    printf("\t.align 4\n");
    printf("\t.type %s, @object\n", name);
    printf("\t.size %s, 4\n", name);
    printf("%s:\n", name);
    printf("\t.zero 4\n");
}

void print_asm(symbol_table_t* global_scope, iloc_op_list_t* iloc_code)
{
    printf("\t.text\n");

    for (int i = 0; i < global_scope->num_symbols; ++i) {
        symbol_t* symbol = global_scope->symbols[i];
        if (symbol->kind == IDENTIFIER) {
            print_asm_global_var(symbol->lex_value->value);
        } else {
            printf("function not implemented :(\n");
            // TODO: Implement function code generation
        }
    }
}
