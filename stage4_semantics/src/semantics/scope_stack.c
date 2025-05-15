#include "scope_stack.h"

scope_stack_t* scope_stack_new(void)
{
    scope_stack_t* new_stack = NULL;
    new_stack = calloc(1, sizeof(scope_stack_t));
    if (new_stack != NULL) {
        new_stack->num_tables = 0;
        new_stack->tables = NULL;
    }
    return new_stack;
}

void scope_stack_free(scope_stack_t* stack)
{
    int i;
    for (i = 0; i < stack->num_tables; i++) {
        table_free(stack->tables[i]);
    }
    free(stack->tables);
    free(stack);
}

void scope_push(scope_stack_t* stack)
{
    if (stack != NULL) {
        symbol_table_t* new_scope = table_new();
        stack->num_tables++;
        stack->tables = realloc(stack->tables, stack->num_tables * sizeof(symbol_table_t*));
        stack->tables[stack->num_tables - 1] = new_scope;
    } else {
        printf("Error: %s received NULL scope stack = %p.\n", __FUNCTION__, stack);
    }
}

void scope_pop(scope_stack_t* stack)
{
    if (stack != NULL) {
        table_free(stack->tables[stack->num_tables - 1]);
        stack->num_tables--;
        stack->tables = realloc(stack->tables, stack->num_tables * sizeof(symbol_table_t*));
    } else {
        printf("Error: %s received NULL scope stack = %p.\n", __FUNCTION__, stack);
    }
}

void scope_add_symbol(scope_stack_t* stack, symbol_t* symbol)
{
    if (stack != NULL && stack->num_tables > 0) {
        table_add_symbol(stack->tables[stack->num_tables - 1], symbol);
    } else {
        printf("Error: %s called with invalid stack or empty scope stack.\n", __FUNCTION__);
    }
}

void scope_stack_debug_print(scope_stack_t* stack)
{
    if (!stack) {
        printf("[Debug] Scope stack is NULL.\n");
        return;
    }

    printf("========== Scope Stack (Top to Bottom) ==========\n");

    for (int i = stack->num_tables - 1; i >= 0; i--) {
        printf("┌── Scope Level %d ───────────────────────────────\n", i);
        symbol_table_debug_print(stack->tables[i]);
        if (i > 0)
            printf(
                "└───────────────────────────────────────────────┐\n"
                "                 ↓\n");
        else
            printf("└───────────────────────────────────────────────┘\n");
    }

    printf("=================================================\n");
}
