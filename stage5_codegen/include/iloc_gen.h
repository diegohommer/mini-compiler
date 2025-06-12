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
typedef struct iloc_op_t {
    char* opcode;            /**< Operation code (e.g., "add", "load", "cbr") */
    char* operand1;          /**< First operand (register, label, constant, etc.) */
    char* operand2;          /**< Second operand (or empty, depending on the instruction) */
    char* operand3;          /**< Third operand (or empty, depending on the instruction) */
    struct iloc_op_t* next;  // pointer to next instruction
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

/**
 * @brief Creates a new ILOC operation.
 *
 * Allocates and initializes an ILOC operation with the given opcode and operands.
 * All strings are duplicated internally.
 *
 * @param opcode  The operation code (e.g., "add", "load", "cbr").
 * @param op1     First operand (register, label, constant, etc.), or NULL.
 * @param op2     Second operand, or NULL.
 * @param op3     Third operand, or NULL.
 * @return Pointer to a newly allocated iloc_op_t structure, or NULL on failure.
 */
iloc_op_t* iloc_op_new(const char* opcode, const char* op1, const char* op2, const char* op3);

/**
 * @brief Frees an ILOC operation.
 *
 * Releases all memory associated with the given instruction, including duplicated strings.
 *
 * @param op Pointer to the iloc_op_t structure to free.
 */
void iloc_op_free(iloc_op_t* op);

/**
 * @brief Creates a new ILOC operation list.
 *
 * Allocates and initializes an empty ILOC operation list.
 *
 * @return Pointer to a newly allocated iloc_op_list_t structure, or NULL on failure.
 */
iloc_op_list_t* iloc_op_list_new(void);

/**
 * @brief Frees all ILOC operations in the list and the list structure itself.
 *
 * This function iterates over the linked list of ILOC operations, frees each operation,
 * and then frees the list container. After this call, the list pointer should not be used.
 *
 * @param list Pointer to the ILOC operation list to be freed. If NULL, the function does nothing.
 */
void iloc_op_list_free(iloc_op_list_t* list);

/**
 * @brief Adds an ILOC operation to a given list of operations.
 *
 * @param list Pointer to the ILOC operation list.
 * @param op Pointer to the ILOC operation to be added.
 */
void iloc_op_list_add_op(iloc_op_list_t* list, iloc_op_t* op);

/**
 * @brief Concatenates two ILOC operation lists by appending the right list to the left.
 *
 * The left list will contain all operations after concatenation,
 * and the right list is cleared (head, tail set to NULL; count set to 0).
 *
 * If the left list is NULL or empty, returns the right list.
 * If the right list is NULL or empty, returns the left list.
 *
 * @param left_list  The list to which operations are appended.
 * @param right_list The list to append and then clear.
 * @return Pointer to the resulting concatenated list.
 */
iloc_op_list_t* iloc_op_list_concat(iloc_op_list_t* left_list, iloc_op_list_t* right_list);

/**
 * @brief Prints all ILOC instructions in the list for debugging purposes.
 *
 * This function iterates over the linked list of ILOC operations
 * and prints the opcode and operands of each instruction.
 * It is intended as a simple debug helper and does not format output.
 *
 * @param list Pointer to the ILOC operation list to print. If NULL, prints a message.
 */
void print_iloc_list_debug(const iloc_op_list_t* list);

#endif  // ILOC_GEN_H
