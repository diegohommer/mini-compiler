#include "symbol_table.h"

symbol_table_t* table_new(void)
{
    symbol_table_t* new_table = NULL;
    new_table = calloc(1, sizeof(symbol_table_t));
    if (new_table != NULL){
        new_table->num_symbols = 0;
        new_table->symbols = NULL;
    }
    return new_table;
}

void table_free(symbol_table_t* table)
{
    if (table != NULL) {
        int i;
        for (i = 0; i < table->num_symbols; i++) {
            symbol_free(table->symbols[i]);
        }
        free(table->symbols);
        free(table);
    } else {
        printf("Error: %s received NULL symbol table = %p.\n", __FUNCTION__, table);
    }
}

void table_add_symbol(symbol_table_t* table, symbol_t* symbol)
{
    if (table != NULL && symbol != NULL) {
        table->num_symbols++;
        table->symbols = realloc(table->symbols, table->num_symbols * sizeof(symbol_t*));
        table->symbols[table->num_symbols - 1] = symbol;
    } else {
        printf("Error: %s received NULL symbol table = %p / %p.\n", __FUNCTION__, table, symbol);
    }
}

symbol_t* symbol_new(kind_t kind, int type, lexical_value_t* lex_value)
{
    symbol_t* symbol = NULL;
    symbol = calloc(1, sizeof(symbol_t));
    if (symbol != NULL) {
        symbol->kind = kind;
        symbol->type = type;
        symbol->params = NULL;
        symbol->lex_value = lex_value;
    }
    return symbol;
}

void symbol_free(symbol_t* symbol)
{
    if (symbol != NULL) {
        if (symbol->params != NULL) {
            if (symbol->kind != FUNCTION) {
                printf("Error: symbol of kind %d has parameters, but is not a FUNCTION\n",
                       symbol->kind);
            }

            int i;
            for (i = 0; i < symbol->params->num_parameters; i++) {
                free(symbol->params->parameters[i]->label);
                free(symbol->params->parameters[i]);
            }
            free(symbol->params->parameters);
            free(symbol->params);
        }

        // Don't free lexical value since asd_free() already does that for us.
        free(symbol);
    }
}

void symbol_add_parameter(symbol_t* symbol, parameter_t* param)
{
    if (symbol == NULL || param == NULL) {
        printf("Error: %s received NULL symbol or param\n", __FUNCTION__);
        return;
    }

    if (symbol->kind != FUNCTION) {
        printf("Error: %s called on non-function (%d) symbol\n", __FUNCTION__, symbol->kind);
        return;
    }

    if (symbol->params == NULL) {
        symbol->params = calloc(1, sizeof(parameters_t));
        if (symbol->params == NULL) {
            printf("Error: failed to allocate parameters_t\n");
            exit(EXIT_FAILURE);
        }
    }

    symbol->params->num_parameters++;
    int num_params = symbol->params->num_parameters;

    symbol->params->parameters =
        realloc(symbol->params->parameters, num_params * sizeof(parameter_t*));
    symbol->params->parameters[num_params - 1] = param;
}