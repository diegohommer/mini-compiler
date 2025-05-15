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
    if (stack == NULL) {
        printf("Error: %s received NULL scope stack = %p.\n", __FUNCTION__, stack);
        return;
    }

    symbol_table_t* new_scope = table_new();
    stack->num_tables++;
    stack->tables = realloc(stack->tables, stack->num_tables * sizeof(symbol_table_t*));
    stack->tables[stack->num_tables - 1] = new_scope;
}

void scope_pop(scope_stack_t* stack)
{
    if (stack == NULL) {
        printf("Error: %s received NULL scope stack = %p.\n", __FUNCTION__, stack);
        return;
    }

    table_free(stack->tables[stack->num_tables - 1]);
    stack->num_tables--;
    stack->tables = realloc(stack->tables, stack->num_tables * sizeof(symbol_table_t*));
}

void scope_declare_symbol(scope_stack_t* stack, symbol_t* symbol)
{
    if (stack == NULL || stack->num_tables == 0) {
        printf("Error: %s called with invalid stack or empty scope stack.\n", __FUNCTION__);
        return;
    }

    table_add_symbol(stack->tables[stack->num_tables - 1], symbol);
}

void scope_declare_function_parameter(scope_stack_t* stack, symbol_t* param_symbol)
{
    if (stack == NULL || stack->num_tables < 2) {
        printf(
            "Error: %s Not enough scopes to find function symbol (expected parent scope below "
            "parameters).\n",
            __FUNCTION__);
        return;
    }

    symbol_table_t* function_decl_table = stack->tables[stack->num_tables - 2];
    if (function_decl_table->num_symbols == 0) {
        printf("Error: %s expected at least one symbol in the function declaration scope.\n",
               __FUNCTION__);
        return;
    }

    symbol_t* func_symbol = function_decl_table->symbols[function_decl_table->num_symbols - 1];
    if (func_symbol->kind != FUNCTION) {
        printf("Error: %s expected last symbol in function declaration scope to be a function.\n",
               __FUNCTION__);
        return;
    }

    parameter_t* new_param = parameter_new(param_symbol->lex_value->value, param_symbol->type);

    symbol_add_parameter(func_symbol, new_param);
    table_add_symbol(stack->tables[stack->num_tables - 1], param_symbol);
}

void scope_stack_debug_print(scope_stack_t* stack)
{
    if (!stack) {
        printf("[Debug] Scope stack is NULL.\n");
        return;
    }

    printf("\n\n========== Scope Stack (Top to Bottom) ==========\n");

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

    printf("=================================================\n\n");
}
