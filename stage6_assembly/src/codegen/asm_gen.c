/*
 * Developed by: Diego Hommerding Amorim - 00341793
 *           Gabriel Kenji Yatsuda Ikuta - 00337491
 */
#include "asm_gen.h"

#include "asd.h"
#include "iloc_ir.h"
#include "symbol_table.h"

/**
 * @brief Array of strings representing the mnemonic names of Assembler opcodes.
 *
 * Indexed by `opcode_t`.
 * Each entry corresponds to the textual opcode name without operand formatting.
 */
static const char* asm_opcode_names[] = {
    [OP_INVALID] = "invalid", [OP_NOP] = "nop",   [OP_HALT] = "jmp end", [OP_ADD] = "addl",
    [OP_SUB] = "subl",        [OP_MULT] = "imul", [OP_DIV] = "idiv",     [OP_AND] = "and",
    [OP_OR] = "or",           [OP_XORI] = "xor",  [OP_LOADAI] = "mov",   [OP_LOADI] = "mov",
    [OP_STOREAI] = "mov",     [OP_I2I] = "mov",   [OP_JUMPI] = "jmp"};

void print_asm_global_var(const char* name)
{
    printf("\t.align 4\n");
    printf("%s:\n\t.zero 4\n", name);
}

void print_iloc_to_asm(iloc_op_t* op)
{
    switch (op->opcode) {
        case OP_NOP:
        case OP_HALT:
            printf("\t%s\n", asm_opcode_names[op->opcode]);
            break;

        case OP_ADD:
        case OP_SUB:
            printf("\tmovl r%d(%%rip), %%eax\n", op->operand1);
            printf("\t%s r%d(%%rip), %%eax\n", asm_opcode_names[op->opcode], op->operand2);
            printf("\tmovl %%eax, r%d(%%rip)\n", op->operand3);
            break;

        default:
            break;
    }
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

    // 2. Print virtual registers for ILOC code
    char reg_name[20];
    if (iloc_code != NULL) {
        for (int r = 0; r < iloc_code->count; ++r) {
            sprintf(reg_name, "r%d", r);  // Convert int to "rN"
            print_asm_global_var(reg_name);
        }
    }

    // 3. Print code segment (main)
    printf("\t.text\n");
    printf("\t.globl main\n");
    printf("\t.type main, @function\n");
    printf("main:\n");
    printf("\tpushq %%rbp\n");
    printf("\tmovq %%rsp, %%rbp\n");

    if (iloc_code != NULL) {
        iloc_op_t* op = iloc_code->head;
        while (op != NULL) {
            print_iloc_to_asm(op);
            op = op->next;
        }
    }

    // Only set return value to 0 if no user return value is generated:
    printf("\tmovl $0, %%eax\n");

    printf("end:\n");
    printf("\tpopq %%rbp\n");
    printf("\tret\n");
}
