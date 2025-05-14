#include "errors.h"

#include <stdio.h>

void display_undeclared_error(const char* label, int line)
{
    printf("Line %d: Error %d - Identifier %s not declared.\n", line, ERR_UNDECLARED, label);
}

void display_declared_error(const char* label, int line)
{
    printf("Line %d: Error %d - Identifier %s already declared.\n", line, ERR_DECLARED, label);
}

void display_variable_error(const char* label, int line)
{
    printf("Line %d: Error %d - Identifier %s is not a variable.\n", line, ERR_VARIABLE, label);
}

void display_function_error(const char* label, int line)
{
    printf("Line %d: Error %d - Identifier %s is not a function.\n", line, ERR_FUNCTION, label);
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
