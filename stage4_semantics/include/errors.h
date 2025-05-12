#ifndef ERROR_H
#define ERROR_H

#define ERR_UNDECLARED       10 //2.2
#define ERR_DECLARED         11 //2.2
#define ERR_VARIABLE         20 //2.3
#define ERR_FUNCTION         21 //2.3
#define ERR_WRONG_TYPE       30 //2.4
#define ERR_MISSING_ARGS     40 //2.5
#define ERR_EXCESS_ARGS      41 //2.5
#define ERR_WRONG_TYPE_ARGS  42 //2.5

void display_undeclared_error(int line_number);
void display_declared_error(int line_number);
void display_variable_error(int line_number);
void display_function_error(int line_number);
void display_wrong_type_error(int line_number);
void display_missing_args_error(int line_number);
void display_excess_args_error(int line_number);
void display_wrong_type_args_error(int line_number);

#endif // ERROR_H
