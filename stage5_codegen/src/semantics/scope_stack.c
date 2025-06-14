/*
 * Developed by: Diego Hommerding Amorim - 00341793
 *           Gabriel Kenji Yatsuda Ikuta - 00337491
 */

#include "scope_stack.h"

#include <stdio.h>
#include <stdlib.h>

#include "errors.h"

scope_stack_t* scope_stack_new(void)
{
    scope_stack_t* new_stack = NULL;
    new_stack = calloc(1, sizeof(scope_stack_t));
    if (new_stack != NULL) {
        new_stack->tables = NULL;
        new_stack->num_tables = 0;
        new_stack->rfp = 0;
        new_stack->rbss = 0;
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

symbol_t* scope_declare_symbol(scope_stack_t* stack, symbol_t* symbol)
{
    if (stack == NULL || stack->num_tables == 0) {
        printf("Error: %s called with invalid stack or empty scope stack.\n", __FUNCTION__);
        return NULL;
    }

    symbol_table_t* current_scope = stack->tables[stack->num_tables - 1];
    symbol_t* declared_symbol = table_add_symbol(current_scope, symbol);

    // If failed to add to table, symbol was already declared
    if (declared_symbol != NULL) {
        display_declared_error(symbol->lex_value->value, symbol->lex_value->line,
                               declared_symbol->lex_value->line);
        CLEAN_EXIT(stack, ERR_DECLARED);
    }

    // If symbol is a variable (not function or literal), assign memory offset
    if (symbol->kind == IDENTIFIER) {
        if (stack->num_tables == 1) {
            // Global variable -> .bss segment
            stack->rbss += INT_SIZE;
            symbol->offset = stack->rbss;
        } else {
            // Local variable -> relative to frame pointer
            stack->rfp += INT_SIZE;
            symbol->offset = stack->rfp;
        }
    }

    return symbol;
}

void scope_declare_function_parameter(scope_stack_t* stack, symbol_t* param_symbol)
{
    scope_declare_symbol(stack, param_symbol);

    symbol_t* func_symbol = scope_get_current_function(stack);
    parameter_t* new_param = parameter_new(param_symbol->lex_value->value, param_symbol->type);
    symbol_add_parameter(func_symbol, new_param);
}

symbol_t* scope_get_current_function(scope_stack_t* stack)
{
    if (stack == NULL || stack->num_tables < 2) {
        printf(
            "Error: %s Not enough scopes to find function symbol (expected parent scope below "
            "parameters).\n",
            __FUNCTION__);
        return NULL;
    }

    symbol_table_t* function_decl_table = stack->tables[0];
    if (function_decl_table->num_symbols == 0) {
        printf("Error: %s expected at least one symbol in the function declaration scope.\n",
               __FUNCTION__);
        return NULL;
    }

    symbol_t* func_symbol = function_decl_table->symbols[function_decl_table->num_symbols - 1];
    if (func_symbol->kind != FUNCTION) {
        printf("Error: %s expected last symbol in function declaration scope to be a function.\n",
               __FUNCTION__);
        return NULL;
    }

    return func_symbol;
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
