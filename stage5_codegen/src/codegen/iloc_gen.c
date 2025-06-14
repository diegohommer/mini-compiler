#include "iloc_gen.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int label_counter = 1;
static int temp_counter = 1;

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

void print_iloc_op(const iloc_op_t* op)
{
    if (op == NULL) {
        printf("NULL instruction\n");
        return;
    }

    const iloc_opcode_format_t* fmt = &opcode_formats[op->opcode];
    switch (fmt->param_count) {
        case 0:
            printf("%s\n", fmt->format);
            break;
        case 1:
            printf(fmt->format, op->operand1);
            printf("\n");
            break;
        case 2:
            printf(fmt->format, op->operand1, op->operand2);
            printf("\n");
            break;
        case 3:
            printf(fmt->format, op->operand1, op->operand2, op->operand3);
            printf("\n");
            break;
        default:
            printf("Invalid param_count %d for opcode %d\n", fmt->param_count, op->opcode);
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
