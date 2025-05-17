#include "errors.h"

#include <stdio.h>

void display_undeclared_error(const char* label, int line)
{
    fprintf(stderr, "Line %d: Error %d (UNDECLARED_ERROR) - Identifier '%s' was not declared.\n",
            line, ERR_UNDECLARED, label);
}

void display_declared_error(const char* label, int line, int decl_line)
{
    fprintf(stderr,
            "Line %d: Error %d (REDECLARED_ERROR) - Identifier '%s' already declared at line %d.\n",
            line, ERR_DECLARED, label, decl_line);
}

void display_variable_error(const char* label, int line, int decl_line)
{
    fprintf(stderr,
            "Line %d: Error %d (VAR_AS_FUNC_ERROR) - Identifier '%s' is used as a function but "
            "was declared as a variable at line %d.\n",
            line, ERR_VARIABLE, label, decl_line);
}

void display_function_error(const char* label, int line, int decl_line)
{
    fprintf(stderr,
            "Line %d: Error %d (FUNC_AS_VAR_ERROR) - Identifier '%s' is used as a variable but "
            "was declared as a function at line %d.\n",
            line, ERR_FUNCTION, label, decl_line);
}

void display_initialization_type_error(int init_line, const char* var_name, int declared_type,
                                       int initializer_type)
{
    fprintf(
        stderr,
        "Line %d: Error %d (TYPE_MISMATCH_ERROR) - Cannot initialize variable '%s' of type '%s' "
        "with value of type '%s'.\n",
        init_line, ERR_WRONG_TYPE, var_name, type_to_string(declared_type),
        type_to_string(initializer_type));
}

void display_atribution_type_error(int assign_line, int decl_line, const char* var_name,
                                   int declared_type, int assigned_type)
{
    fprintf(stderr,
            "Line %d: Error %d (TYPE_MISMATCH_ERROR) - Cannot assign type '%s' to variable '%s' "
            "(declared at line %d as '%s').\n",
            assign_line, ERR_WRONG_TYPE, type_to_string(assigned_type), var_name, decl_line,
            type_to_string(declared_type));
}

void display_return_type_error(int return_line, int func_line, const char* func_name, int type_decl,
                               int type_used)
{
    fprintf(stderr,
            "Line %d: Error %d (TYPE_MISMATCH_ERROR) - Return type mismatch in function '%s' "
            "declared at line %d: expected '%s' but got '%s'.\n",
            return_line, ERR_WRONG_TYPE, func_name, func_line, type_to_string(type_decl),
            type_to_string(type_used));
}

void display_if_else_type_error(int line, int type_if, int type_else)
{
    fprintf(stderr,
            "Line %d: Error %d (TYPE_MISMATCH_ERROR) - Mismatched types between 'if' and 'else' "
            "blocks: '%s' vs '%s'.\n",
            line, ERR_WRONG_TYPE, type_to_string(type_if), type_to_string(type_else));
}

void display_expression_type_error(int line, const char* op, int type1, int type2)
{
    fprintf(stderr,
            "Line %d: Error %d (TYPE_MISMATCH_ERROR) - Cannot apply '%s' between '%s' and '%s'.\n",
            line, ERR_WRONG_TYPE, op, type_to_string(type1), type_to_string(type2));
}

void display_missing_args_error(int call_line, int decl_line, const char* func_name, int expected,
                                int got)
{
    fprintf(stderr,
            "Line %d: Error %d (ERR_MISSING_ARGS) - Too few arguments to function '%s' declared at "
            "line %d; expected %d but got %d.\n",
            call_line, ERR_MISSING_ARGS, func_name, decl_line, expected, got);
}

void display_excess_args_error(int call_line, int decl_line, const char* func_name, int expected,
                               int got)
{
    fprintf(stderr,
            "Line %d: Error %d (ERR_EXCESS_ARGS) - Too many arguments to function '%s' declared at "
            "line %d; expected %d but got %d.\n",
            call_line, ERR_EXCESS_ARGS, func_name, decl_line, expected, got);
}

void display_wrong_type_args_error(int call_line, int decl_line, const char* func_name,
                                   int param_index, const char* arg_label, int expected_type,
                                   int provided_type)
{
    fprintf(stderr,
            "Line %d: Error %d (ERR_WRONG_TYPE_ARGS) - Expected '%s' but got '%s' for argument %d "
            "('%s') of function '%s' declared at line %d.\n",
            call_line, ERR_WRONG_TYPE_ARGS, type_to_string(expected_type),
            type_to_string(provided_type), param_index, arg_label, func_name, decl_line);
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
