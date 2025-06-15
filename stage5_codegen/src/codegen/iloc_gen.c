#include "iloc_gen.h"

#include <stdlib.h>
#include <string.h>

#include "iloc_ir.h"

void iloc_gen_store(asd_tree_t* output, int src_temp, int var_offset, int num_scope_tables)
{
    // Store variable: mem[offset] = src_temp (in RFP or RBSS)
    iloc_op_list_t* code = iloc_op_list_new();
    iloc_op_t* iloc_op = NULL;

    if (num_scope_tables == 1) {
        // Global scope: use RBSS
        iloc_op = iloc_op_new(OP_STOREAI, src_temp, RBSS_ID, var_offset);
    } else {
        // Local scope: use RFP
        iloc_op = iloc_op_new(OP_STOREAI, src_temp, RFP_ID, var_offset);
    }
    iloc_op_list_add_op(code, iloc_op);

    output->code = code;
}

void iloc_gen_unary_exp(asd_tree_t* output, const char* op_token, asd_tree_t* operand)
{
    int temp = 0;
    iloc_op_list_t* code = operand->code;
    iloc_op_t* op = NULL;

    if (strcmp(op_token, "+") == 0) {
        // Unary plus: no operation
        temp = operand->temp;
    } else if (strcmp(op_token, "-") == 0) {
        // Unary minus: temp = operand * -1
        temp = temp_new();
        op = iloc_op_new(OP_MULTI, operand->temp, -1, temp);
        iloc_op_list_add_op(code, op);
    } else if (strcmp(op_token, "!") == 0) {
        // Logical NOT: temp = operand XOR -1 (bitwise inversion)
        temp = temp_new();
        op = iloc_op_new(OP_XORI, operand->temp, -1, temp);
        iloc_op_list_add_op(code, op);
    }

    output->code = code;
    output->temp = temp;
}

void iloc_gen_binary_exp(asd_tree_t* output, asd_tree_t* left, const char* op_token,
                         asd_tree_t* right)
{
    int temp = 0;
    opcode_t opcode = operator_to_opcode(op_token);
    iloc_op_list_t* code = iloc_op_list_concat(left->code, right->code);

    if (opcode != OP_INVALID) {
        // Generate binary operation: temp = left <op> right
        temp = temp_new();
        iloc_op_t* op = iloc_op_new(opcode, left->temp, right->temp, temp);
        iloc_op_list_add_op(code, op);
    } else {
        // Fallback: propagate left operand (e.g., error case)
        temp = left->temp;
    }

    output->code = code;
    output->temp = temp;
}

void iloc_gen_literal(asd_tree_t* output, const char* value)
{
    // Load immediate: temp = value
    int temp = temp_new();
    iloc_op_list_t* code = iloc_op_list_new();
    iloc_op_t* iloc_op = iloc_op_new(OP_LOADI, atoi(value), temp, UNUSED_OPERAND);
    iloc_op_list_add_op(code, iloc_op);

    output->code = code;
    output->temp = temp;
}
