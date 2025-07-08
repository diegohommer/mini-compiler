/*
 * Developed by: Diego Hommerding Amorim - 00341793
 *           Gabriel Kenji Yatsuda Ikuta - 00337491
 */

#ifndef ILOC_IR_H
#define ILOC_IR_H

#include <stdio.h>

#define RBSS_ID -2
#define RFP_ID  -1

#define UNUSED_OP 0

#define INT_SIZE 4

/**
 * @enum opcode_t
 * @brief Enumeration of all supported ILOC opcodes.
 *
 * Each opcode corresponds to a specific ILOC instruction,
 * representing arithmetic, logical, control flow, and memory operations.
 */
typedef enum {
    OP_NOP,     /**< nop */
    OP_HALT,    /**< halt */
    OP_ADD,     /**< add r1, r2 => r3 */
    OP_SUB,     /**< sub r1, r2 => r3 */
    OP_MULT,    /**< mult r1, r2 => r3 */
    OP_DIV,     /**< div r1, r2 => r3 */
    OP_MULTI,   /**< multI r1, c2 => r3 */
    OP_AND,     /**< and r1, r2 => r3 */
    OP_OR,      /**< or r1, r2 => r3 */
    OP_XORI,    /**< xorI r1, c2 => r3 */
    OP_LOADAI,  /**< loadAI r1, c2 => r2 */
    OP_LOADI,   /**< loadI c1 => r2 */
    OP_STOREAI, /**< storeAI r1 => r2, c3 */
    OP_I2I,     /**< i2i r1 => r2 */
    OP_JUMPI,   /**< jumpI -> l1 */
    OP_CBR,     /**< cbr r1 -> l2, l3 */
    OP_CMP_LT,  /**< cmp_LT r1, r2 -> r3 */
    OP_CMP_LE,  /**< cmp_LE r1, r2 -> r3 */
    OP_CMP_EQ,  /**< cmp_EQ r1, r2 -> r3 */
    OP_CMP_GE,  /**< cmp_GE r1, r2 -> r3 */
    OP_CMP_GT,  /**< cmp_GT r1, r2 -> r3 */
    OP_CMP_NE,  /**< cmp_NE r1, r2 -> r3 */
    OP_LABEL,   /**< l1: */
    OP_INVALID,
} opcode_t;

/**
 * @struct iloc_op_t
 * @brief Represents an ILOC instruction with an opcode and up to three operand IDs.
 */
typedef struct iloc_op_t {
    opcode_t opcode; /**< Operation code (e.g., add, load, cbr) */
    int operand1;    /**< ID of the first operand (optional, depending on the instruction) */
    int operand2;    /**< ID of the second operand (optional, depending on the instruction) */
    int operand3;    /**< ID of the third operand (optional, depending on the instruction) */
    struct iloc_op_t* next; /**< Pointer to the next instruction in the list */
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
 * @brief Generates a new unique label ID.
 * @return Label ID (integer).
 */
int label_new(void);

/**
 * @brief Generates a new unique temporary ID.
 * @return Temporary ID (integer).
 */
int temp_new(void);

/**
 * @brief Maps a high-level operator string to its corresponding ILOC opcode.
 *
 * Converts a high-level operator (e.g., "+", "-", "*", "/") into the corresponding
 * internal ILOC opcode used for code generation.
 *
 * @param op A null-terminated string representing the high-level operator.
 * @return The corresponding ILOC opcode as an opcode_t value,
 *         or -1 (or another sentinel value) if the operator is not recognized.
 */
opcode_t operator_to_opcode(const char* op);

/**
 * @brief Creates a new ILOC operation.
 *
 * Allocates and initializes an ILOC operation with the given opcode and operand IDs.
 *
 * @param opcode  The operation code (e.g., ADD, LOAD, CBR) as an opcode_t value.
 * @param op1     ID of the first operand (register, label, constant).
 * @param op2     ID of the second operand (register, label, constant).
 * @param op3     ID of the third operand (register, label, constant).
 * @return Pointer to a newly allocated iloc_op_t structure, or NULL on failure.
 */
iloc_op_t* iloc_op_new(opcode_t opcode, int op1, int op2, int op3);

/**
 * @brief Frees an ILOC operation.
 *
 * Releases all memory associated with the given instruction.
 *
 * @param op Pointer to the iloc_op_t structure to free.
 */
void iloc_op_free(iloc_op_t* op);

/**
 * @brief Writes the string representation of a register operand into the provided buffer.
 *
 * Special register IDs like `rfp` and `rbss` are mapped to their names.
 * Other registers are represented as `rX` where X is the register ID.
 *
 * @param reg_id Register ID.
 * @param buf Buffer to write the string into.
 * @param bufsize Size of the buffer.
 * @return Pointer to the buffer containing the register name.
 *
 * @note The buffer must be large enough to hold the string.
 */
const char* iloc_reg_to_str(int reg_id, char* buf, size_t bufsize);

/**
 * @brief Prints a single ILOC instruction formatted according to its opcode.
 *
 * This function uses the opcode's format string and the operands from the
 * iloc_op_t to print the instruction in human-readable form.
 *
 * @param op Pointer to the ILOC operation to print.
 */
void print_iloc_op(const iloc_op_t* op);

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
 * @param list Pointer to the ILOC operation list to be freed. If NULL, the function does
 * nothing.
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
 * This function appends all operations from @p right_list to @p left_list.
 * The @p right_list container is freed after concatenation, but its operations
 * are preserved within the resulting list.
 *
 * - If @p left_list is NULL or empty, the function frees it and returns @p right_list.
 * - If @p right_list is NULL or empty, the function frees it and returns @p left_list.
 * - If both lists are non-empty, @p right_list is appended to @p left_list, and
 *   the @p right_list container is freed.
 *
 * @param left_list  The destination list to which operations are appended.
 * @param right_list The source list whose operations are appended; its container is freed.
 * @return Pointer to the resulting concatenated list.
 */
iloc_op_list_t* iloc_op_list_concat(iloc_op_list_t* left_list, iloc_op_list_t* right_list);

/**
 * @brief Prints all ILOC instructions in the list as formatted code.
 *
 * This function iterates over the linked list of ILOC operations,
 * formats each instruction according to its opcode and operands,
 * and outputs them line-by-line, appending a newline after each.
 *
 * The output corresponds to the final ILOC code representation,
 * suitable for debugging or emitting to a file.
 *
 * @param list Pointer to the ILOC operation list to print.
 *             If NULL or empty, no output is produced.
 */
void print_iloc_list(const iloc_op_list_t* list);

#endif  // ILOC_IR_H
