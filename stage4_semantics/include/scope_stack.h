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
 * @brief Declares a new symbol in the current scope.
 *
 * Exits with ERR_DECLARED if already declared in this scope.
 *
 * @param stack Pointer to the scope stack.
 * @param symbol Symbol to declare.
 */
void scope_declare_symbol(scope_stack_t* stack, symbol_t* symbol);

/**
 * @brief Declares a function parameter to the function currently being defined.
 *
 * This function assumes that:
 * - The function symbol was added to the scope *below* the current one.
 * - The parameter symbol is already created but not yet added to any scope.
 *
 * It adds the parameter symbol both to the current scope (as a local variable)
 * and to the parameter list of the function symbol (in the parent scope).
 *
 * If the parent scope is empty or does not contain a function symbol as the last entry,
 * this function will raise an internal error.
 *
 * @param stack Pointer to the current scope stack.
 * @param param_symbol Pointer to the parameter symbol to be added.
 */
void scope_declare_function_parameter(scope_stack_t* stack, symbol_t* param_symbol);

/**
 * @brief Searches for a symbol by label in the scope stack.
 *
 * Searches from the innermost scope to the outermost.
 *
 * @param stack Pointer to the current scope stack.
 * @param label The identifier name of the symbol.
 * @param line The line where the symbol is being used.
 * @return Pointer to the found symbol_t, or NULL if not found.
 */
symbol_t* scope_get_symbol(scope_stack_t* stack, const char* label, int line);

/**
 * @brief Validates if a symbol is being used correctly based on its declaration.
 *
 * Exits with ERR_UNDECLARED if the symbol is not declared in any scope.
 * Exits with ERR_VARIABLE if the symbol is declared as a variable but is being used as a function.
 * Exits with ERR_FUNCTION if the symbol is declared as a function but is being used as a variable.
 * Exits with ERR_WRONG_TYPE if the symbol has a different type than expected for its usage.
 *
 * @param stack Pointer to the current scope stack.
 * @param used_symbol Pointer to the symbol to be validated.
 */
void scope_validate_symbol_usage(scope_stack_t* stack, symbol_t* used_symbol);

/**
 * @brief Prints the contents of the scope stack for debugging purposes.
 *
 * Each scope is printed with its level and the symbols it contains.
 *
 * @param stack Pointer to the scope stack.
 */
void scope_stack_debug_print(scope_stack_t* stack);

#endif  // SCOPE_STACK_H
