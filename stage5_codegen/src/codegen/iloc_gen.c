#include "iloc_gen.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int label_counter = 1;
static int temp_counter = 1;

/**
 * @brief Array of strings representing the mnemonic names of ILOC opcodes.
 *
 * Indexed by `opcode_t`.
 * Each entry corresponds to the textual opcode name without operand formatting.
 */
static const char* opcode_names[] = {
    [OP_INVALID] = "invalid", [OP_NOP] = "nop",       [OP_ADD] = "add",
    [OP_SUB] = "sub",         [OP_MULT] = "mult",     [OP_DIV] = "div",
    [OP_MULTI] = "multI",     [OP_AND] = "and",       [OP_OR] = "or",
    [OP_XORI] = "xorI",       [OP_LOADAI] = "loadAI", [OP_LOADI] = "loadI",
    [OP_STOREAI] = "storeAI", [OP_JUMPI] = "jumpI",   [OP_CBR] = "cbr",
    [OP_CMP_LT] = "cmp_LT",   [OP_CMP_LE] = "cmp_LE", [OP_CMP_EQ] = "cmp_EQ",
    [OP_CMP_GE] = "cmp_GE",   [OP_CMP_GT] = "cmp_GT", [OP_CMP_NE] = "cmp_NE",
};

int temp_new(void) { return temp_counter++; }

int label_new(void) { return label_counter++; }

opcode_t operator_to_opcode(const char* op)
{
    if (strcmp(op, "+") == 0) return OP_ADD;
    if (strcmp(op, "-") == 0) return OP_SUB;
    if (strcmp(op, "*") == 0) return OP_MULT;
    if (strcmp(op, "/") == 0) return OP_DIV;
    if (strcmp(op, "==") == 0) return OP_CMP_EQ;
    if (strcmp(op, "!=") == 0) return OP_CMP_NE;
    if (strcmp(op, "<") == 0) return OP_CMP_LT;
    if (strcmp(op, ">") == 0) return OP_CMP_GT;
    if (strcmp(op, "<=") == 0) return OP_CMP_LE;
    if (strcmp(op, ">=") == 0) return OP_CMP_GE;
    if (strcmp(op, "&") == 0) return OP_AND;
    if (strcmp(op, "|") == 0) return OP_OR;
    return OP_INVALID;
}

iloc_op_t* iloc_op_new(opcode_t opcode, int op1, int op2, int op3)
{
    iloc_op_t* op = malloc(sizeof(iloc_op_t));
    if (!op) {
        return NULL;
    }

    op->opcode = opcode;
    op->operand1 = op1;
    op->operand2 = op2;
    op->operand3 = op3;
    op->next = NULL;

    return op;
}

void iloc_op_free(iloc_op_t* op)
{
    if (!op) {
        return;
    }
    free(op);
}

const char* iloc_reg_to_str(int reg_id, char* buf, size_t bufsize)
{
    if (reg_id == RBSS_ID) {
        snprintf(buf, bufsize, "rbss");
    } else if (reg_id == RFP_ID) {
        snprintf(buf, bufsize, "rfp");
    } else {
        snprintf(buf, bufsize, "r%d", reg_id);
    }
    return buf;
}

void print_iloc_op(const iloc_op_t* op)
{
    if (op == NULL) {
        printf("NULL instruction\n");
        return;
    }

    // Buffers for storing possible register 'rX' strings
    char buf1[16], buf2[16], buf3[16];

    switch (op->opcode) {
        case OP_NOP:
            // opcode: nop
            printf("nop\n");
            break;

        case OP_ADD:
        case OP_SUB:
        case OP_MULT:
        case OP_DIV:
        case OP_AND:
        case OP_OR:
            // opcode r1, r2 => r3
            iloc_reg_to_str(op->operand1, buf1, sizeof(buf1));
            iloc_reg_to_str(op->operand2, buf2, sizeof(buf2));
            iloc_reg_to_str(op->operand3, buf3, sizeof(buf3));
            printf("%s %s, %s => %s\n", opcode_names[op->opcode], buf1, buf2, buf3);
            break;

        case OP_MULTI:
        case OP_XORI:
        case OP_LOADAI:
        case OP_STOREAI:
            // opcode r1, c2 => r3
            iloc_reg_to_str(op->operand1, buf1, sizeof(buf1));
            iloc_reg_to_str(op->operand3, buf3, sizeof(buf3));
            printf("%s %s, %d => %s\n", opcode_names[op->opcode], buf1, op->operand2, buf3);
            break;

        case OP_LOADI:
            // opcode c1 => r2
            iloc_reg_to_str(op->operand2, buf2, sizeof(buf2));
            printf("%s %d => %s\n", opcode_names[op->opcode], op->operand1, buf2);
            break;

        case OP_JUMPI:
            // opcode -> l1
            printf("%s -> l%d\n", opcode_names[op->opcode], op->operand1);
            break;

        case OP_CBR:
            // opcode r1 -> l2, l3
            iloc_reg_to_str(op->operand1, buf1, sizeof(buf1));
            printf("%s %s -> l%d, l%d\n", opcode_names[op->opcode], buf1, op->operand2,
                   op->operand3);
            break;

        case OP_CMP_LT:
        case OP_CMP_LE:
        case OP_CMP_EQ:
        case OP_CMP_GE:
        case OP_CMP_GT:
        case OP_CMP_NE:
            // opcode r1, r2 -> r3
            iloc_reg_to_str(op->operand1, buf1, sizeof(buf1));
            iloc_reg_to_str(op->operand2, buf2, sizeof(buf2));
            iloc_reg_to_str(op->operand3, buf3, sizeof(buf3));
            printf("%s %s, %s -> %s\n", opcode_names[op->opcode], buf1, buf2, buf3);
            break;

        default:
            printf("Unknown opcode %d\n", op->opcode);
            break;
    }
}

iloc_op_list_t* iloc_op_list_new(void)
{
    iloc_op_list_t* list = malloc(sizeof(iloc_op_list_t));
    if (!list) {
        return NULL;
    }
    list->count = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void iloc_op_list_free(iloc_op_list_t* list)
{
    if (!list) {
        return;
    }

    iloc_op_t* current = list->head;
    iloc_op_t* next = NULL;

    while (current != NULL) {
        next = current->next;
        iloc_op_free(current);
        current = next;
    }

    free(list);
}

void iloc_op_list_add_op(iloc_op_list_t* list, iloc_op_t* op)
{
    if (list->count == 0) {
        list->head = op;
        list->tail = op;
    } else {
        list->tail->next = op;
        list->tail = op;
    }
    list->count++;
}

iloc_op_list_t* iloc_op_list_concat(iloc_op_list_t* left_list, iloc_op_list_t* right_list)
{
    if (!left_list || left_list->count == 0) {
        // Left empty or NULL — just return right
        return right_list;
    }
    if (!right_list || right_list->count == 0) {
        // Right empty or NULL — just return left
        return left_list;
    }

    // Connect the two lists
    left_list->tail->next = right_list->head;
    left_list->tail = right_list->tail;
    left_list->count += right_list->count;

    // Clear right list to avoid dangling pointers
    right_list->head = NULL;
    right_list->tail = NULL;
    right_list->count = 0;

    return left_list;
}

void print_iloc_list(const iloc_op_list_t* list)
{
    if (list == NULL) {
        printf("ILOC list is NULL\n");
        return;
    }

    iloc_op_t* current = list->head;
    while (current != NULL) {
        print_iloc_op(current);
        current = current->next;
    }
}
