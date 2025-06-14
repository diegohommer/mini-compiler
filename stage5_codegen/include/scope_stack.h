/*
 * Developed by: Diego Hommerding Amorim - 00341793
 *           Gabriel Kenji Yatsuda Ikuta - 00337491
 */

#ifndef SCOPE_STACK_H
#define SCOPE_STACK_H

#define CLEAN_EXIT(stack, code)  \
    do {                         \
        scope_stack_free(stack); \
        exit(code);              \
    } while (0)

#include "symbol_table.h"

/**
 * @struct scope_stack_t
 * @brief Stack of symbol tables representing nested scopes.
 */
typedef struct {
    symbol_table_t** tables; /**< Array of symbol table pointers, one per scope. */
    int num_tables;          /**< Number of scopes currently in the stack. */
    int rfp;                 /**< Current frame pointer offset for local variables */
    int rbss;                /**< Current offset for global variables */
} scope_stack_t;

/**
 * @brief Resets the frame pointer offset to zero.
 *
 * @param stack Pointer to the scope stack.
 */
static inline void scope_stack_reset_rfp(scope_stack_t* stack) { stack->rfp = 0; }

/**
 * @brief Creates and initializes an empty scope stack.
 *
 * @return Pointer to a new scope stack.
 */
scope_stack_t* scope_stack_new(void);

/**
 * @brief Frees the scope stack and all contained symbol tables.
 *
 * @param stack Pointer to the scope stack to free.
 */
void scope_stack_free(scope_stack_t* stack);

/**
 * @brief Pushes a new empty scope onto the stack.
 *
 * @param stack Pointer to the scope stack.
 */
void scope_push(scope_stack_t* stack);

/**
 * @brief Pops and frees the top scope from the stack.
 *
 * @param stack Pointer to the scope stack.
 */
void scope_pop(scope_stack_t* stack);

/**
 * @brief Declares a new symbol in the current (top) scope.
 *
 * @param stack Pointer to the scope stack.
 * @param symbol Symbol to declare.
 *
 * @note Exits with ERR_DECLARED if the symbol is already declared in this scope.
 */
void scope_declare_symbol(scope_stack_t* stack, symbol_t* symbol);

/**
 * @brief Declares a function parameter for the function currently being defined.
 *
 * Assumes the function symbol is in the scope below the current one.
 * Adds the parameter symbol to both the current scope and the function's parameter list.
 *
 * @param stack Pointer to the scope stack.
 * @param param_symbol Parameter symbol to declare.
 *
 */
void scope_declare_function_parameter(scope_stack_t* stack, symbol_t* param_symbol);

/**
 * @brief Retrieves the function symbol of the function whose scope is currently active.
 *
 * Assumes that the function symbol is stored in the scope immediately below the current scope,
 * and is the last symbol declared in that scope.
 *
 * @param stack Pointer to the scope stack.
 * @return Pointer to the function symbol currently being defined, or NULL if not found.
 */
symbol_t* scope_get_current_function(scope_stack_t* stack);

/**
 * @brief Searches for a symbol by label in the scope stack.
 *
 * Searches from the innermost scope outward.
 *
 * @param stack Pointer to the scope stack.
 * @param label Identifier name of the symbol.
 * @param line Line number where the symbol is used.
 * @return Pointer to the found symbol.
 *
 * @note Exits with ERR_UNDECLARED if the symbol is not found.
 */
symbol_t* scope_get_symbol(scope_stack_t* stack, const char* label, int line);

/**
 * @brief Prints the scope stack and symbols for debugging.
 *
 * Shows each scope level and its contained symbols.
 *
 * @param stack Pointer to the scope stack.
 */
void scope_stack_debug_print(scope_stack_t* stack);

#endif  // SCOPE_STACK_H
