#ifndef SCOPE_STACK_H
#define SCOPE_STACK_H

#include "symbol_table.h"

/**
 * @struct scope_stack_t
 * @brief Represents a stack of symbol tables used for managing nested scopes.
 */
typedef struct {
	symbol_table_t** tables; /**< Array of pointers to symbol tables (one per scope). */
	int num_tables;          /**< Current number of tables (scopes) in the stack. */
} scope_stack_t;

/**
 * @brief Creates and initializes an empty scope stack.
 *
 * @return Pointer to a newly allocated scope stack.
 */
scope_stack_t* scope_stack_new(void);

/**
 * @brief Frees all memory associated with the scope stack and its symbol tables.
 *
 * @param stack Pointer to the scope stack to be freed.
 */
void scope_stack_free(scope_stack_t* stack);

/**
 * @brief Pushes a new scope onto the stack.
 *
 * Creates a new, empty symbol table and appends it to the stack.
 *
 * @param stack Pointer to the scope stack.
 */
void scope_push(scope_stack_t* stack);

/**
 * @brief Pops the top scope from the stack and frees its contents.
 *
 * @param stack Pointer to the scope stack.
 */
void scope_pop(scope_stack_t* stack);

/**
 * @brief Adds a symbol to the current (top) scope on the stack.
 *
 * Raises ERR_DECLARED if a symbol with the same label already exists in the current scope.
 *
 * @param stack Pointer to the scope stack.
 * @param symbol Pointer to the symbol to be added.
 */
void scope_add_symbol(scope_stack_t* stack, symbol_t* symbol);

#endif  // SCOPE_STACK_H

