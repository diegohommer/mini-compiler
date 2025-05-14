#ifndef SCOPE_STACK_H
#define SCOPE_STACK_H

#include "symbol_table.h"

typedef struct {
    symbol_table_t** tables;
    int num_tables;
} scope_stack_t;

/*
 * Function scope_stack_new, creates an empty symbol table stack.
 */
scope_stack_t* scope_stack_new(void);

/*
 * Function scope_stack_free, recursively frees the given stack and its tables.
 */
void scope_stack_free(scope_stack_t* stack);

/*
 * Function scope_push, creates a new scope (symbol table) and appends it to the given stack.
 */
void scope_push(scope_stack_t* stack);

/*
 * Function scope_pop, removes the scope that's on top of the stack and free its contents.
 */
void scope_pop(scope_stack_t* stack);

/*
 * Function scope_add_symbol, adds the given symbol to the scope that's on top of the stack.
 */
void scope_add_symbol(symbol_t* symbol);

#endif  // SCOPE_STACK_H
