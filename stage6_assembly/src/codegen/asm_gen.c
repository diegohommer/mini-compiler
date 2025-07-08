/*
 * Developed by: Diego Hommerding Amorim - 00341793
 *           Gabriel Kenji Yatsuda Ikuta - 00337491
 */
#include "asm_gen.h"

#include <stdlib.h>

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
    [OP_SUB] = "subl",        [OP_MULT] = "imul", [OP_MULTI] = "imul",   [OP_AND] = "and",
    [OP_OR] = "or",           [OP_XORI] = "xor",  [OP_JUMPI] = "jmp"};

const char* asm_reg_to_str(int reg_id, char* buf, size_t bufsize)
{
    if (reg_id == RFP_ID) {
        snprintf(buf, bufsize, "%%rbp");
    } else {
        // Virtual registers use RIP-relative addressing: rN(%rip)
        snprintf(buf, bufsize, "r%d(%%rip)", reg_id);
    }
    return buf;
}

void print_asm_global_var(const char* name)
{
    printf("\t.align 4\n");
    printf("%s:\n\t.zero 4\n", name);
}

void print_iloc_to_asm(iloc_op_t* op, char** globals_by_offset)
{
    char src1[32] = {0};
    char src2[32] = {0};
    char dest[32] = {0};

    if (op->operand1 != UNUSED_OP) asm_reg_to_str(op->operand1, src1, sizeof(src1));
    if (op->operand2 != UNUSED_OP) asm_reg_to_str(op->operand2, src2, sizeof(src2));
    if (op->operand3 != UNUSED_OP) asm_reg_to_str(op->operand3, dest, sizeof(dest));

    switch (op->opcode) {
        case OP_NOP:
        case OP_HALT:
            printf("\t%s\n", asm_opcode_names[op->opcode]);
            break;

        case OP_ADD:
        case OP_SUB:
        case OP_MULT:
        case OP_AND:
        case OP_OR:
            printf("\tmovl %s, %%eax\n", src1);
            printf("\t%s %s, %%eax\n", asm_opcode_names[op->opcode], src2);
            printf("\tmovl %%eax, %s\n", dest);
            break;

        case OP_DIV:
            printf("\tmovl %s, %%eax\n", src1);
            printf("\tcltd\n");
            printf("\tidivl %s\n", src2);
            printf("\tmovl %%eax, %s\n", dest);
            break;

        case OP_MULTI:
        case OP_XORI:
            printf("\tmovl %s, %%eax\n", src1);
            printf("\t%s $%d, %%eax\n", asm_opcode_names[op->opcode], op->operand2);
            printf("\tmovl %%eax, %s\n", dest);
            break;

        case OP_LOADAI:
            if (op->operand1 == RFP_ID) {
                printf("\tmovl %d(%%rbp), %%eax\n", -(op->operand2) - 4);
            } else {
                int var_idx = (op->operand2 / 4);
                printf("\tmovl %s(%%rip), %%eax\n", globals_by_offset[var_idx]);
            }
            printf("\tmovl %%eax, %s\n", dest);
            break;

        case OP_LOADI:
            printf("\tmovl $%d, %s\n", op->operand1, src2);
            break;

        case OP_STOREAI:
            printf("\tmovl %s, %%eax\n", src1);
            if (op->operand2 == RFP_ID) {
                printf("\tmovl %%eax, %d(%%rbp)\n", -(op->operand3) - 4);
            } else {
                int var_idx = (op->operand2 / 4);
                printf("\tmovl %%eax, %s(%%rip)\n", globals_by_offset[var_idx]);
            }
            break;

        default:
            break;
    }
}

void print_asm(symbol_table_t* global_scope, iloc_op_list_t* iloc_code)
{
    // 0. Create vector to reference all global variables
    char** globals_by_offset = malloc(sizeof(char*) * global_scope->num_symbols);
    int pos = 0;

    // 1. Print data segment (globals)
    printf("\t.bss\n");
    for (int i = 0; i < global_scope->num_symbols; ++i) {
        symbol_t* symbol = global_scope->symbols[i];
        if (symbol->kind == IDENTIFIER) {
            print_asm_global_var(symbol->lex_value->value);
            globals_by_offset[pos] = symbol->lex_value->value;
            pos++;
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
            print_iloc_to_asm(op, globals_by_offset);
            op = op->next;
        }
    }

    // Only set return value to 0 if no user return value is generated:
    printf("\tmovl $0, %%eax\n");

    printf("end:\n");
    printf("\tpopq %%rbp\n");
    printf("\tret\n");

    free(globals_by_offset);
}
