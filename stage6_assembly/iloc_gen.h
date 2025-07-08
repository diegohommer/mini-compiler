/*
 * Developed by: Diego Hommerding Amorim - 00341793
 *           Gabriel Kenji Yatsuda Ikuta - 00337491
 */

#ifndef ILOC_GEN_H
#define ILOC_GEN_H

#include "asd.h"

/**
 * @brief Generates ILOC code to store the value of an expression into a
 *        variable's memory location.
 *
 * @param output            Pointer to the AST node to which the generated
 *                          ILOC code will be appended.
 * @param exp               Pointer to the AST node representing the value
 *                          to be stored (expression or literal).
 * @param var_offset        Memory offset of the variable within the stack frame.
 * @param var_level         Used to determine if the variable is global (RBSS)
 *                          or local (RFP).
 */
void iloc_gen_store(asd_tree_t* output, asd_tree_t* exp, int var_offset, int var_level);

/**
 * @brief Generates ILOC code to load the value of a variable into a
 *        temp register.
 *
 * @param output            Pointer to the AST node to which the generated
 *                          ILOC code will be appended.
 * @param var_offset        Memory offset of the variable within the stack frame.
 * @param var_level         Used to determine if the variable is global (RBSS)
 *                          or local (RFP).
 */
void iloc_gen_load(asd_tree_t* output, int var_offset, int var_level);

/**
 * @brief Generates ILOC code for an if-else control structure.
 *
 * Generates conditional branching code based on the result of the
 * evaluated expression. If the condition is true, the 'if_block'
 * is executed; otherwise, the 'else_block' is executed (if present).
 *
 * @param output     Pointer to the AST node to which the generated
 *                   ILOC code will be appended.
 * @param exp        AST node containing the condition expression
 *                   and its generated code.
 * @param if_block   AST node containing the code to execute if
 *                   the condition is true.
 * @param else_block AST node containing the code to execute if
 *                   the condition is false. Can be NULL if no
 *                   else block is present.
 */
void iloc_gen_if(asd_tree_t* output, asd_tree_t* exp, asd_tree_t* if_block, asd_tree_t* else_block);

/**
 * @brief Generates ILOC code for a while loop structure.
 *
 * @param output     Pointer to the AST node to which the generated
 *                          ILOC code will be appended.
 * @param exp        The expression node representing the loop condition.
 * @param cmd_block  The command block node representing the loop body.
 */
void iloc_gen_while(asd_tree_t* output, asd_tree_t* exp, asd_tree_t* cmd_block);

/**
 * @brief Generates ILOC code for a unary expression and appends the instructions
 *        to the provided AST node's code list.
 *
 * @param output    Pointer to the AST node where generated code is appended.
 * @param op_token  The unary operator as a string (e.g., "-", "!").
 * @param operand   The AST node for the operand expression.
 */
void iloc_gen_unary_exp(asd_tree_t* output, const char* op_token, asd_tree_t* operand);

/**
 * @brief Generates ILOC code for a binary expression and appends the instructions
 *        to the provided AST node's code list.
 *
 * @param output    Pointer to the AST node where generated code is appended.
 * @param left      The AST node representing the left operand expression.
 * @param op_token  The binary operator as a string (e.g., "+", "-", "/", "==").
 * @param right     The AST node representing the right operand expression.
 */
void iloc_gen_binary_exp(asd_tree_t* output, asd_tree_t* left, const char* op_token,
                         asd_tree_t* right);

/**
 * @brief Generates ILOC code for a literal value by loading it into a new temporary,
 *        and appends the instructions to the provided AST node's code list.
 *
 * @param output  Pointer to the AST node where generated code is appended.
 * @param value   The string representation of the literal value to load.
 */
void iloc_gen_literal(asd_tree_t* output, const char* value);

/**
 * @brief Generates ILOC code for returning from the main function.
 *        It moves the return value (assumed in a temporary) to r1.
 *
 * @param output     Pointer to the AST node where generated code is appended.
 * @param exp        Pointer to the AST node of the return expression.
 */
void iloc_gen_return(asd_tree_t* output, asd_tree_t* exp);

#endif  // ILOC_GEN_H
