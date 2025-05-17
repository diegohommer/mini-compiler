#include "errors.h"

#include <stdio.h>

void display_undeclared_error(const char* label, int line)
{
    printf("Line %d: Error %d (UNDECLARED_ERROR) - Identifier %s was not declared.\n", line,
           ERR_UNDECLARED, label);
}

void display_declared_error(const char* label, int line, int decl_line)
{
    printf("Line %d: Error %d (REDECLARED_ERROR)- Identifier %s already declared at line %d.\n",
           line, ERR_DECLARED, label, decl_line);
}

void display_variable_error(const char* label, int line, int decl_line)
{
    printf(
        "Line %d: Error %d (VAR_AS_FUNC_ERROR)- Identifier '%s' is used as a function but was "
        "declared as a variable at line %d.\n",
        line, ERR_VARIABLE, label, decl_line);
}

void display_function_error(const char* label, int line, int decl_line)
{
    printf(
        "Line %d: Error %d (FUNC_AS_VAR_ERROR) - Identifier '%s' is used as a variable but was "
        "declared as a function at line %d.\n",
        line, ERR_FUNCTION, label, decl_line);
}

void display_expression_type_error(int line, const char* op, int type1, int type2)
{
    printf("Line %d: Error %d (TYPE_MISMATCH_ERROR)- Cannot apply '%s' between '%s' and '%s'.\n",
           line, ERR_WRONG_TYPE, op, type_to_string(type1), type_to_string(type2));
}

void display_return_type_error(int return_line, int func_line, const char* func_name,
                               int type_decl, int type_used)
{
    printf("Line %d: Error %d (TYPE_MISMATCH_ERROR) - Return type in function '%s' declared at line %d is '%s', but got '%s'.\n",
           return_line, ERR_WRONG_TYPE, func_name, func_line,
           type_to_string(type_decl), type_to_string(type_used));
}

void display_if_else_type_error(int line, int type_if, int type_else)
{
    printf(
        "Line %d: Error %d (TYPE_MISMATCH_ERROR) - Mismatched types between 'if' and 'else' "
        "blocks: '%s' vs '%s'.\n",
        line, ERR_WRONG_TYPE, type_to_string(type_if), type_to_string(type_else));
}

void display_missing_args_error(int line)
{
    printf("Line %d: Error %d - Missing function arguments.\n", line, ERR_MISSING_ARGS);
}

void display_excess_args_error(int line)
{
    printf("Line %d: Error %d - Too many function arguments.\n", line, ERR_EXCESS_ARGS);
}

void display_wrong_type_args_error(int line)
{
    printf("Line %d: Error %d - Incompatible argument types.\n", line, ERR_WRONG_TYPE_ARGS);
}

const char* type_to_string(int type)
{
    switch (type) {
        case 0:
            return "int";
        case 1:
            return "float";
        default:
            return "unknown";
    }
}
