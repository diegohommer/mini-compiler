/*
 * Developed by: Diego Hommerding Amorim - 00341793
 *           Gabriel Kenji Yatsuda Ikuta - 00337491
 */

#include "symbol_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

symbol_table_t* table_new(void)
{
    symbol_table_t* new_table = NULL;
    new_table = calloc(1, sizeof(symbol_table_t));
    if (new_table != NULL) {
        new_table->num_symbols = 0;
        new_table->symbols = NULL;
    }
    return new_table;
}

void table_free(symbol_table_t* table)
{
    if (table == NULL) {
        printf("Error: %s received NULL symbol table = %p.\n", __FUNCTION__, table);
        return;
    }

    int i;
    for (i = 0; i < table->num_symbols; i++) {
        symbol_free(table->symbols[i]);
    }
    free(table->symbols);
    free(table);
}

symbol_t* table_add_symbol(symbol_table_t* table, symbol_t* symbol)
{
    if (table == NULL || symbol == NULL) {
        printf("Error: %s received NULL symbol table = %p / %p.\n", __FUNCTION__, table, symbol);
        return NULL;
    }

    int i;
    // Check if a symbol with the same name has already been delcared in the current scope
    for (i = 0; i < table->num_symbols; i++) {
        if (strcmp(table->symbols[i]->lex_value->value, symbol->lex_value->value) == 0) {
            return table->symbols[i];
        }
    }
    table->num_symbols++;
    table->symbols = realloc(table->symbols, table->num_symbols * sizeof(symbol_t*));
    table->symbols[table->num_symbols - 1] = symbol;
    return NULL;
}

symbol_t* table_get_symbol(symbol_table_t* table, const char* label)
{
    if (table == NULL) {
        printf("Error: %s received NULL symbol table = %p.\n", __FUNCTION__, table);
        return NULL;
    }

    int i;
    // Look for a symbol declared in the table that has the given label/name
    for (i = 0; i < table->num_symbols; i++) {
        if (strcmp(table->symbols[i]->lex_value->value, label) == 0) {
            return table->symbols[i];
        }
    }
    return NULL;
}

symbol_t* symbol_new(kind_t kind, type_t type, lexical_value_t* lex_value)
{
    symbol_t* symbol = NULL;
    symbol = calloc(1, sizeof(symbol_t));
    if (symbol != NULL) {
        symbol->kind = kind;
        symbol->type = type;
        symbol->params = NULL;

        lexical_value_t* local_copy = malloc(sizeof(lexical_value_t));
        local_copy->value = strdup(lex_value->value);
        local_copy->line = lex_value->line;
        symbol->lex_value = local_copy;
        symbol->offset = 0;
        symbol->level = 0;
    }
    return symbol;
}

void symbol_free(symbol_t* symbol)
{
    if (symbol == NULL) {
        return;
    }

    if (symbol->params != NULL) {
        if (symbol->kind != FUNCTION) {
            printf("Error: symbol of kind %d has parameters, but is not a FUNCTION\n",
                   symbol->kind);
        }

        int i;
        for (i = 0; i < symbol->params->num_parameters; i++) {
            parameter_free(symbol->params->parameters[i]);
        }
        free(symbol->params->parameters);
        free(symbol->params);
    }

    free(symbol->lex_value->value);
    free(symbol->lex_value);
    free(symbol);
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

void symbol_table_debug_print(symbol_table_t* table)
{
    if (!table) {
        printf("  [empty symbol table]\n");
        return;
    }

    for (int i = 0; i < table->num_symbols; i++) {
        symbol_t* sym = table->symbols[i];
        printf("  - %s (kind: %d, type: %d, line: %d)\n", sym->lex_value->value, (int)sym->kind,
               (int)sym->type, sym->lex_value->line);

        // If the symbol has parameters, display them
        if (sym->params != NULL) {
            printf("    Parameters:\n");
            for (int j = 0; j < sym->params->num_parameters; j++) {
                parameter_t* param = sym->params->parameters[j];
                printf("      • %s (type: %d)\n", param->label, (int)param->type);
            }
        }
    }
}

parameter_t* parameter_new(const char* label, type_t type)
{
    parameter_t* param = NULL;
    param = calloc(1, sizeof(parameter_t));
    param->label = strdup(label);
    param->type = type;
    return param;
}

void parameter_free(parameter_t* param)
{
    if (param != NULL) {
        free(param->label);
    }
    free(param);
}
