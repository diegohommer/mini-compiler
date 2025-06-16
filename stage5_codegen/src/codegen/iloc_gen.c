#include "iloc_gen.h"

#include <stdlib.h>
#include <string.h>

#include "asd.h"
#include "iloc_ir.h"

void iloc_gen_store(asd_tree_t* output, asd_tree_t* exp, int var_offset, int var_level)
{
    // Store variable: mem[offset] = src_temp (in RFP or RBSS)
    iloc_op_list_t* code = iloc_op_list_new();
    iloc_op_t* iloc_op = NULL;

    if (var_level == 0) {
        // Global scope: use RBSS
        iloc_op = iloc_op_new(OP_STOREAI, exp->temp, RBSS_ID, var_offset);
    } else {
        // Local scope: use RFP
        iloc_op = iloc_op_new(OP_STOREAI, exp->temp, RFP_ID, var_offset);
    }
    iloc_op_list_add_op(code, iloc_op);

    output->code = iloc_op_list_concat(exp->code, code);
}

void iloc_gen_load(asd_tree_t* output, int var_offset, int var_level)
{
    // Load variable: temp = mem[offset] (in RFP or RBSS)
    int temp = temp_new();
    iloc_op_list_t* code = iloc_op_list_new();
    iloc_op_t* iloc_op = NULL;

    if (var_level == 0) {
        // Global scope: use RBSS
        iloc_op = iloc_op_new(OP_LOADAI, RBSS_ID, var_offset, temp);
    } else {
        // Local scope: use RFP
        iloc_op = iloc_op_new(OP_LOADAI, RFP_ID, var_offset, temp);
    }
    iloc_op_list_add_op(code, iloc_op);

    output->code = code;
    output->temp = temp;
}

void iloc_gen_if(asd_tree_t* output, asd_tree_t* exp, asd_tree_t* if_block, asd_tree_t* else_block)
{
    int if_id = label_new();                      // Entry point of the if-block
    int else_id = else_block ? label_new() : -1;  // Entry point of the else-block (if present)
    int end_id = label_new();                     // Exit point after if/else

    // Labels
    iloc_op_t* label_if = iloc_op_new(OP_LABEL, if_id, UNUSED_OP, UNUSED_OP);
    iloc_op_t* label_end = iloc_op_new(OP_LABEL, end_id, UNUSED_OP, UNUSED_OP);

    // Branch: if true -> if-block, else -> else-block or end
    int false_target = else_block ? else_id : end_id;
    iloc_op_t* cbr = iloc_op_new(OP_CBR, exp->temp, if_id, false_target);

    iloc_op_list_t* code = exp->code;  // Condition evaluation
    iloc_op_list_add_op(code, cbr);    // Conditional branch

    // If block
    iloc_op_list_add_op(code, label_if);
    if (if_block) {
        iloc_op_list_concat(code, if_block->code);
    }

    if (else_block) {
        // Jump to end after if-block to skip else-block
        iloc_op_list_add_op(code, iloc_op_new(OP_JUMPI, end_id, UNUSED_OP, UNUSED_OP));

        // Else block
        iloc_op_list_add_op(code, iloc_op_new(OP_LABEL, else_id, UNUSED_OP, UNUSED_OP));
        iloc_op_list_concat(code, else_block->code);
    }

    // Common exit point
    iloc_op_list_add_op(code, label_end);

    output->code = code;
}

void iloc_gen_while(asd_tree_t* output, asd_tree_t* exp, asd_tree_t* cmd_block)
{
    int start_id = label_new();  // Loop condition label
    int body_id = label_new();   // Loop body label
    int end_id = label_new();    // Loop exit label

    iloc_op_list_t* code = iloc_op_list_new();

    // Labels
    iloc_op_t* label_start = iloc_op_new(OP_LABEL, start_id, UNUSED_OP, UNUSED_OP);
    iloc_op_t* label_body = iloc_op_new(OP_LABEL, body_id, UNUSED_OP, UNUSED_OP);
    iloc_op_t* label_end = iloc_op_new(OP_LABEL, end_id, UNUSED_OP, UNUSED_OP);

    // Conditional branch: if condition true -> body, else -> end
    iloc_op_t* cbr = iloc_op_new(OP_CBR, exp->temp, body_id, end_id);

    // Jump back to condition after body
    iloc_op_t* jump_back = iloc_op_new(OP_JUMPI, start_id, UNUSED_OP, UNUSED_OP);

    iloc_op_list_add_op(code, label_start);
    iloc_op_list_concat(code, exp->code);
    iloc_op_list_add_op(code, cbr);

    iloc_op_list_add_op(code, label_body);
    if (cmd_block) {
        // Only access cmd_block if node is not NULL
        iloc_op_list_concat(code, cmd_block->code);
    }
    iloc_op_list_add_op(code, jump_back);

    iloc_op_list_add_op(code, label_end);

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
    iloc_op_t* iloc_op = iloc_op_new(OP_LOADI, atoi(value), temp, UNUSED_OP);
    iloc_op_list_add_op(code, iloc_op);

    output->code = code;
    output->temp = temp;
}
