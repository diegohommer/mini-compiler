/*
 * Developed by: Diego Hommerding Amorim - 00341793
 *           Gabriel Kenji Yatsuda Ikuta - 00337491
 */

#ifndef ERROR_H
#define ERROR_H

// Semantic error codes
#define ERR_UNDECLARED      10
#define ERR_DECLARED        11
#define ERR_VARIABLE        20
#define ERR_FUNCTION        21
#define ERR_WRONG_TYPE      30
#define ERR_MISSING_ARGS    40
#define ERR_EXCESS_ARGS     41
#define ERR_WRONG_TYPE_ARGS 42

// Error display functions
void display_undeclared_error(const char* label, int line_number);
void display_declared_error(const char* label, int used_line_number, int declared_line_number);
void display_variable_error(const char* label, int used_line_number, int declared_line_number);
void display_function_error(const char* label, int used_line_number, int declared_line_number);
void display_initialization_type_error(int init_line, const char* var_name, int declared_type,
                                       int initializer_type);
void display_atribution_type_error(int assign_line, int decl_line, const char* var_name,
                                   int declared_type, int assigned_type);
void display_return_type_error(int return_line, int func_line, const char* func_name, int type_decl,
                               int type_used);
void display_if_else_type_error(int if_line, int type_if, int type_else);
void display_expression_type_error(int line, const char* op, int type_left, int type_right);
void display_missing_args_error(int call_line, int decl_line, const char* func_name, int expected,
                                int got);
void display_excess_args_error(int call_line, int decl_line, const char* func_name, int expected,
                               int got);
void display_wrong_type_args_error(int call_line, int decl_line, const char* func_name,
                                   int param_index, const char* param_name, int expected_type,
                                   int provided_type);

// Utils
const char* type_to_string(int type);

#endif  // ERROR_H
