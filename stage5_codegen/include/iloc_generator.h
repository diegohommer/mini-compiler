/*
 * Developed by: Diego Hommerding Amorim - 00341793
 *           Gabriel Kenji Yatsuda Ikuta - 00337491
 */

#ifndef ILOC_GENERATOR_H
#define ILOC_GENERATOR_H

/**
 * @struct iloc_op_t
 * @brief Represents an ILOC instruction with an opcode and up to three operands/parameters.
 */
typedef struct {
    char* opcode; /**< Operation code (e.g., "add", "load", "cbr") */
    char* param1; /**< First operand (register, label, constant, etc.) */
    char* param2; /**< Second operand (or empty, depending on the instruction) */
    char* param3; /**< Third operand (or empty, depending on the instruction) */
} iloc_op_t;

/**
 * @struct iloc_op_list_t
 * @brief Represents a linked list of ILOC instructions.
 */
typedef struct {
    iloc_op_t* head; /**< Pointer to the first instruction */
    iloc_op_t* tail; /**< Pointer to the last instruction */
    int count;       /**< Number of instructions in the list */
} iloc_op_list_t;

#endif  // ILOC_GENERATOR_H
