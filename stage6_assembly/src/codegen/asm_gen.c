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
    printf("\t.align 4\n");
    printf("\t.type %s, @object\n", name);
    printf("\t.size %s, 4\n", name);
    printf("%s:\n", name);
    printf("\t.zero 4\n");
}

void print_asm(symbol_table_t* global_scope, iloc_op_list_t* iloc_code)
{
    // 1. Print data segment (globals)
    printf("\t.bss\n");
    for (int i = 0; i < global_scope->num_symbols; ++i) {
        symbol_t* symbol = global_scope->symbols[i];
        if (symbol->kind == IDENTIFIER) {
            print_asm_global_var(symbol->lex_value->value);
        }
    }

    // 2. Print code segment (main)
    printf("\t.text\n");
    printf("\t.globl main\n");
    printf("\t.type main, @function\n");
    printf("main:\n");
    printf("\tpushq %%rbp\n");
    printf("\tmovq %%rsp, %%rbp\n");

    // TODO: insert user code here (e.g. translate ILOC instructions)

    // Only set return value to 0 if no user return value is generated:
    printf("\tmovl $0, %%eax\n");

    printf("\tpopq %%rbp\n");
    printf("\tret\n");
}
