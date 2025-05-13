#ifndef TABLE_H
#define TABLE_H

#include "asd.h"

typedef enum { LITERAL, IDENTIFIER, FUNCTION } kind_t;

/* Structure for storing function parameters */
typedef struct {
    char* label;
    type_t type;
} parameter_t;

/* Structure for holding a list of parameters for a function */
typedef struct {
    int num_parameters;
    parameter_t** parameters;
} parameters_t;

/* Structure representing a symbol in the symbol table */
typedef struct {
    kind_t kind;
    type_t type;
    parameters_t* params;
    lexical_value_t* lex_value;
} symbol_t;

/* Structure representing a symbol table */
typedef struct {
    symbol_t** symbols;
    int num_symbols;
} symbol_table_t;

/*
 * Function table_new, creates a symbol table without symbols.
 */
symbol_table_t* table_new(void);

/*
 * Function table_free, recursively frees the table and its symbols.
 */
void table_free(symbol_table_t* table);

/*
 * Function table_add_symbol, adds a given symbol to the given table
 */
void table_add_symbol(symbol_table_t* table, symbol_t* symbol);

/*
 * Function symbol_new, creates a symbol with the given fields and an empty param list.
 */
symbol_t* symbol_new(kind_t kind, int type, lexical_value_t* lex_value);

/*
 * Function symbol_free, recursively frees the symbol and its parameters.
 */
void symbol_free(symbol_t* symbol);

/*
 * Function symbol_add_parameter, adds a given parameter to the given symbol parameters list.
 */
void symbol_add_parameter(symbol_t* symbol, parameter_t* param);

#endif  // TABLE_H
