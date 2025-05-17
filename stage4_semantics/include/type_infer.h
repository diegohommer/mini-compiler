#ifndef TYPE_INFER_H
#define TYPE_INFER_H

#include "scope_stack.h"

/**
 * @brief Infers and validates the resulting type of a binary expression.
 *
 * Compares the data types of the left and right expression trees.
 *
 * @param scope_tack
 * @param op         The operator being applied (e.g., "+", "==", "|").
 * @param exp_left   Pointer to the left operand's AST node.
 * @param exp_right  Pointer to the right operand's AST node.
 * @return The common data type if both operands have the same type.
 * @note Exits with ERR_WRONG_TYPE if the types do not match.
 */
type_t infer_exp_type(scope_stack_t* stack, const char* op, asd_tree_t* exp_left,
                      asd_tree_t* exp_right);

/**
 * @brief Infers and validates the resulting type of an if-else command.
 *
 * Compares the data types of the `if` and `else` blocks (if `else_block` is not NULL).
 *
 * @param cond_type   The type inferred from the if-else condition expression.
 * @param if_block    AST node representing the `if` block.
 * @param else_block  AST node representing the `else` block (may be NULL).
 * @return The condition expression type, if blocks are compatible.
 * @note Exits with ERR_WRONG_TYPE if the block types do not match.
 */
type_t infer_if_type(scope_stack_t* stack, type_t cond_type, asd_tree_t* if_block,
                     asd_tree_t* else_block);

#endif  // TYPE_INFER_H
