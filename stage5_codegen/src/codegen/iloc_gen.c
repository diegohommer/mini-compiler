#include <iloc_gen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int label_counter = 0;
static int temp_counter = 0;

char* temp_new(void)
{
    int length = snprintf(NULL, 0, "r%d", temp_counter);
    char* name = malloc(length + 1);
    if (!name)
        return NULL;
    sprintf(name, "r%d", temp_counter++);
    return name;
}

char* label_new(void)
{
    int length = snprintf(NULL, 0, "l%d", label_counter);
    char* name = malloc(length + 1);
    if (!name)
        return NULL;
    sprintf(name, "l%d", label_counter++);
    return name;
}

iloc_op_t* iloc_op_new(const char* opcode, const char* op1, const char* op2, const char* op3)
{
    iloc_op_t* op = malloc(sizeof(iloc_op_t));
    if (!op) {
        return NULL;
    }
    op->opcode = opcode ? strdup(opcode) : NULL;
    op->operand1 = op1 ? strdup(op1) : NULL;
    op->operand2 = op2 ? strdup(op2) : NULL;
    op->operand3 = op3 ? strdup(op3) : NULL;
    op->next = NULL;

    return op;
}

void iloc_op_free(iloc_op_t* op)
{
    if (!op) {
        return;
    }
    free(op->opcode);
    free(op->operand1);
    free(op->operand2);
    free(op->operand3);
    free(op);
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
