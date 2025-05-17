#include "errors.h"

#include <stdio.h>

void display_undeclared_error(const char* label, int line)
{
    printf("Line %d: Error %d - Identifier %s was not declared.\n", line, ERR_UNDECLARED, label);
}

void display_declared_error(const char* label, int line, int decl_line)
{
    printf("Line %d: Error %d - Identifier %s already declared at line %d .\n", line, ERR_DECLARED,
           label, decl_line);
}

void display_variable_error(const char* label, int line, int decl_line)
{
    printf(
        "Line %d: Error %d - Identifier '%s' is used as a function but was declared as a "
        "variable at line %d.\n",
        line, ERR_VARIABLE, label, decl_line);
}

void display_function_error(const char* label, int line, int decl_line)
{
    printf(
        "Line %d: Error %d - Identifier '%s' is used as a variable but was declared as a "
        "function at line %d.\n",
        line, ERR_VARIABLE, label, decl_line);
}

void display_wrong_type_error(int line)
{
    printf("Line %d: Error %d - Incompatible types.\n", line, ERR_WRONG_TYPE);
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
