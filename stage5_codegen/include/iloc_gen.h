/*
 * Developed by: Diego Hommerding Amorim - 00341793
 *           Gabriel Kenji Yatsuda Ikuta - 00337491
 */

#ifndef ILOC_GEN_H
#define ILOC_GEN_H

/**
 * @struct iloc_op_t
 * @brief Represents an ILOC instruction with an opcode and up to three operands/parameters.
 */
typedef struct {
    char* opcode;   /**< Operation code (e.g., "add", "load", "cbr") */
    char* operand1; /**< First operand (register, label, constant, etc.) */
    char* operand2; /**< Second operand (or empty, depending on the instruction) */
    char* operand3; /**< Third operand (or empty, depending on the instruction) */
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

/**
 * @brief Creates a new unique label name in the format "lX",
 * where X is an incrementing number.
 * @return Dynamically allocated label string (must be freed).
 */
char* label_new(void);

/**
 * @brief Creates a new unique temporary name in the format "rX",
 * where X is an incrementing number.
 * @return Dynamically allocated temp string (must be freed).
 */
char* temp_new(void);

#endif  // ILOC_GEN_H
