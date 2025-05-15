#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string.h>

#include "asd.h"
#include "errors.h"

/**
 * @enum type_t
 * @brief Represents the type of a symbol (e.g., INT or FLOAT).
 */
typedef enum { INT = 0, FLOAT = 1 } type_t;

/**
 * @struct parameter_t
 * @brief Represents a parameter of a function.
 */
typedef struct {
    char* label; /**< Label (identifier) of the parameter */
    type_t type; /**< Type of the parameter (INT or FLOAT) */
} parameter_t;

/**
 * @struct parameters_t
 * @brief Represents a list of function parameters.
 */
typedef struct {
    int num_parameters;       /**< Number of parameters in the list */
    parameter_t** parameters; /**< Parameter vector */
} parameters_t;

/**
 * @struct symbol_t
 * @brief Represents a symbol entry in the symbol table.
 */
typedef struct {
    kind_t kind;                /**< Kind of the symbol (IDENTIFIER, LITERAL or FUNCTION). */
    type_t type;                /**< Type of the symbol (INT or FLOAT). */
    parameters_t* params;       /**< Function parameters if symbol is a function, NULL otherwise. */
    lexical_value_t* lex_value; /**< Lexical value (identifier, line info, etc.). */
} symbol_t;

/**
 * @struct symbol_table_t
 * @brief Represents a symbol table.
 */
typedef struct {
    symbol_t** symbols;
    int num_symbols;
} symbol_table_t;

/**
 * @brief Creates and initializes an empty symbol table.
 *
 * @return Pointer to a newly allocated symbol table.
 */
symbol_table_t* table_new(void);

/**
 * @brief Frees all memory associated with the symbol table and its contents.
 *
 * @param table Pointer to the symbol table to be freed.
 */
void table_free(symbol_table_t* table);

/**
 * @brief Adds a symbol to the symbol table.
 *
 * Checks if the symbol has already been declared by comparing its label with
 * existing entries. If a duplicate is found, raises ERR_DECLARED.
 *
 * @param table Pointer to the symbol table.
 * @param symbol Pointer to the symbol to be added.
 */
void table_add_symbol(symbol_table_t* table, symbol_t* symbol);

/**
 * @brief Searches for a symbol in the symbol table.
 *
 * Compares the label of the given symbol against all entries in the table.
 *
 * @param table Pointer to the symbol table.
 * @param symbol Pointer to the symbol to search for.
 * @return Pointer to the matching symbol if found, NULL otherwise.
 */
symbol_t* table_lookup_symbol(symbol_table_t* table, symbol_t* symbol);

/**
 * @brief Creates a new symbol with the given kind, type, and lexical value.
 *
 * A deep copy of the given lexical value is created and stored within the symbol.
 * The parameter list is initialized to empty (NULL).
 *
 * @param kind The kind of the symbol (IDENTIFIER, LITERAL, or FUNCTION).
 * @param type The type of the symbol (INT, FLOAT, etc.).
 * @param lex_value Pointer to the lexical value to be copied.
 * @return Pointer to a newly allocated symbol containing its own internal copy of the lexical value.
 */
symbol_t* symbol_new(kind_t kind, type_t type, lexical_value_t* lex_value);

/**
 * @brief Frees memory associated with the given symbol.
 *
 * Recursively frees parameter list (if any) and the internal copy of the lexical value.
 * Does not affect the original lexical value passed to `symbol_new`, as a deep copy
 * was made during construction.
 *
 * @param symbol Pointer to the symbol to be freed.
 */
void symbol_free(symbol_t* symbol);

/**
 * @brief Adds a parameter to a function symbol's parameter list.
 *
 * If the parameter list is NULL, a new one is created.
 *
 * @param symbol Pointer to the symbol (must be a function).
 * @param param Pointer to the parameter to be added.
 */
void symbol_add_parameter(symbol_t* symbol, parameter_t* param);

/**
 * @brief Prints the contents of the symbol table for debugging purposes.
 *
 * Each symbol is displayed with relevant metadata such as label, type, and declaration line.
 *
 * @param table Pointer to the symbol table to print.
 */
void symbol_table_debug_print(symbol_table_t* table);

#endif  // SYMBOL_TABLE_H
