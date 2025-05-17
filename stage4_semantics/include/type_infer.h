#ifndef TYPE_INFER_H
#define TYPE_INFER_H

#include "scope_stack.h"

/**
 * @brief Infers and validates the resulting type of a binary expression.
 *
 * Compares the data types of the left and right expression trees.
 *
 * @param scope_stack Pointer to the scope stack.
 * @param op          The operator being applied (e.g., "+", "==", "|").
 * @param exp_left    Pointer to the left operand's AST node.
 * @param exp_right   Pointer to the right operand's AST node.
 * @return The common data type if both operands have the same type.
 *
 * @note Exits with ERR_WRONG_TYPE if the types do not match.
 */
type_t infer_exp_type(scope_stack_t* scope_stack, const char* op, asd_tree_t* exp_left,
                      asd_tree_t* exp_right);

/**
 * @brief Infers and validates the return type of a function call expression.
 *
 * Checks if the called function exists, and verifies the number and types
 * of arguments against the function's parameter list as registered in the symbol table.
 *
 * @param scope_stack Pointer to the scope stack.
 * @param func_name   Name of the function being called.
 * @param call_args   AST node representing the list of arguments in the function call.
 * @return The return type of the function if the call is valid.
 *
 * @note Exits with ERR_UNDECLARED if the function is not declared.
 * @note Exits with ERR_VARIABLE if the identifier used is that of a variable.
 * @note Exits with ERR_MISSING_ARGS if too few arguments are provided.
 * @note Exits with ERR_EXCESS_ARGS if too many arguments are provided.
 * @note Exits with ERR_WRONG_TYPE_ARGS if any argument type does not match the expected parameter type.
 */
type_t infer_function_call_type(scope_stack_t* scope_stack, lexical_value_t* call_id, asd_tree_t* call_args);

/**
 * @brief Infers and validates the return expression type considering user declaration and function
 * return type.
 *
 * @param scope_stack    Current scope for semantic info.
 * @param return_expr    AST node for the return expression.
 * @param declared_type  Type explicitly declared on the return expression by the user.
 *
 * @return The final resolved type of the return expression if compatible.
 * @note Exits with ERR_WRONG_TYPE if the expression type doesn't match the declared return type.
 * @note Exits with ERR_WRONG_TYPE if the expression type doesn't match the function's return type.
 */
type_t infer_return_type(scope_stack_t* scope_stack, asd_tree_t* return_expr, type_t declared_type);

/**
 * @brief Infers and validates the resulting type of an if-else command.
 *
 * Compares the data types of the `if` and `else` blocks (if `else_block` is not NULL).
 *
 * @param scope_stack Pointer to the scope_stack.
 * @param cond_type   The type inferred from the if-else condition expression.
 * @param if_block    AST node representing the `if` block.
 * @param else_block  AST node representing the `else` block (may be NULL).
 * @return The condition expression type, if blocks are compatible.
 *
 * @note Exits with ERR_WRONG_TYPE if the block types do not match.
 */
type_t infer_if_type(scope_stack_t* scope_stack, type_t cond_type, asd_tree_t* if_block,
                     asd_tree_t* else_block);

#endif  // TYPE_INFER_H
