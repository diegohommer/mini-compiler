/*
 * Developed by: Diego Hommerding Amorim - 00341793
 *           Gabriel Kenji Yatsuda Ikuta - 00337491
 */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "asd.h"

/**
 * @struct parameter_t
 * @brief Represents a function parameter.
 */
typedef struct {
    char* label; /**< Parameter identifier */
    type_t type; /**< Parameter data type (INT, FLOAT, etc.) */
} parameter_t;

/**
 * @struct parameters_t
 * @brief Represents a list of function parameters.
 */
typedef struct {
    int num_parameters;       /**< Number of parameters */
    parameter_t** parameters; /**< Array of pointers to parameters */
} parameters_t;

/**
 * @struct symbol_t
 * @brief Represents an entry in the symbol table.
 */
typedef struct {
    kind_t kind;                /**< Symbol kind (IDENTIFIER, LITERAL, FUNCTION) */
    type_t type;                /**< Symbol data type */
    parameters_t* params;       /**< Function parameters, or NULL if not a function */
    lexical_value_t* lex_value; /**< Lexical value with identifier and position info */
} symbol_t;

/**
 * @struct symbol_table_t
 * @brief Represents a symbol table.
 */
typedef struct {
    symbol_t** symbols; /**< Array of pointers to symbols */
    int num_symbols;    /**< Number of symbols stored */
} symbol_table_t;

/**
 * @brief Creates and initializes an empty symbol table.
 *
 * @return Pointer to a newly allocated symbol table.
 */
symbol_table_t* table_new(void);

/**
 * @brief Frees the symbol table and all associated symbols.
 *
 * @param table Pointer to the symbol table to free.
 */
void table_free(symbol_table_t* table);

/**
 * @brief Adds a symbol to the symbol table if not already declared.
 *
 * If a symbol with the same label exists, returns a pointer to it.
 * Otherwise, adds the new symbol and returns NULL.
 *
 * @param table Pointer to the symbol table.
 * @param symbol Pointer to the symbol to add.
 * @return NULL if symbol was added successfully; pointer to existing symbol if duplicate.
 */
symbol_t* table_add_symbol(symbol_table_t* table, symbol_t* symbol);

/**
 * @brief Searches for a symbol by label in the symbol table.
 *
 * @param table Pointer to the symbol table.
 * @param label Symbol identifier to search for.
 * @return Pointer to the symbol if found, or NULL otherwise.
 */
symbol_t* table_get_symbol(symbol_table_t* table, const char* label);

/**
 * @brief Creates a new symbol with a deep copy of the lexical value.
 *
 * Initializes the parameter list to NULL.
 *
 * @param kind The kind of symbol (IDENTIFIER, LITERAL, FUNCTION).
 * @param type The data type of the symbol.
 * @param lex_value Pointer to the lexical value to copy.
 * @return Pointer to the newly allocated symbol.
 */
symbol_t* symbol_new(kind_t kind, type_t type, lexical_value_t* lex_value);

/**
 * @brief Frees the memory associated with a symbol.
 *
 * Also frees the parameter list (if any) and the copied lexical value.
 *
 * @param symbol Pointer to the symbol to free.
 */
void symbol_free(symbol_t* symbol);

/**
 * @brief Adds a parameter to a function symbol's parameter list.
 *
 * Creates the parameter list if it does not exist.
 *
 * @param symbol Pointer to the function symbol.
 * @param param Pointer to the parameter to add.
 */
void symbol_add_parameter(symbol_t* symbol, parameter_t* param);

/**
 * @brief Prints the symbol table contents for debugging.
 *
 * Displays each symbol's label, type, and other metadata.
 *
 * @param table Pointer to the symbol table.
 */
void symbol_table_debug_print(symbol_table_t* table);

/**
 * @brief Creates a new function parameter.
 *
 * Allocates and initializes a parameter with the given label and type.
 *
 * @param label The parameter's identifier.
 * @param type The parameter's data type.
 * @return Pointer to the newly created parameter.
 */
parameter_t* parameter_new(const char* label, type_t type);

/**
 * @brief Frees a function parameter.
 *
 * @param param Pointer to the parameter to free.
 */
void parameter_free(parameter_t* param);

#endif  // SYMBOL_TABLE_H
