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

    symbol_table_t* current_scope = stack->tables[stack->num_tables - 1];
    symbol_t* declared_symbol = table_add_symbol(current_scope, symbol);

    // If failed to add to table, symbol was already declared
    if (declared_symbol != NULL) {
        display_declared_error(symbol->lex_value->value, symbol->lex_value->line,
                               declared_symbol->lex_value->line);
        CLEAN_EXIT(stack, ERR_DECLARED);
    }
}

symbol_t* scope_get_symbol(scope_stack_t* stack, const char* label, int line)
{
    // Search from innermost to outermost scope
    for (int i = stack->num_tables - 1; i >= 0; i--) {
        symbol_t* declared_symbol = table_get_symbol(stack->tables[i], label);
        if (declared_symbol != NULL) {
            return declared_symbol;
        }
    }

    // Not found: undeclared identifier being used
    display_undeclared_error(label, line);
    CLEAN_EXIT(stack, ERR_UNDECLARED);
}

void scope_validate_symbol_usage(scope_stack_t* stack, symbol_t* used_symbol)
{
    if (stack == NULL || used_symbol == NULL) {
        printf("Error: %s called with invalid stack or symbol.\n", __FUNCTION__);
        return;
    }

    const char* label = used_symbol->lex_value->value;
    int line = used_symbol->lex_value->line;
    symbol_t* declared_symbol =
        scope_get_symbol(stack, used_symbol->lex_value->value, used_symbol->lex_value->line);

    // Found: check for mismatching types and kinds
    if (used_symbol->type != declared_symbol->type) {
        display_wrong_type_error(line);
        symbol_free(used_symbol);
        CLEAN_EXIT(stack, ERR_WRONG_TYPE);
    }
    if (used_symbol->kind == FUNCTION && declared_symbol->kind == IDENTIFIER) {
        display_variable_error(label, line);
        symbol_free(used_symbol);
        CLEAN_EXIT(stack, ERR_VARIABLE);
    }
    if (used_symbol->kind == IDENTIFIER && declared_symbol->kind == FUNCTION) {
        display_function_error(label, line);
        symbol_free(used_symbol);
        CLEAN_EXIT(stack, ERR_FUNCTION);
    }

    // All good
    return;
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
