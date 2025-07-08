/*
 * Developed by: Diego Hommerding Amorim - 00341793
 *           Gabriel Kenji Yatsuda Ikuta - 00337491
 */

#ifndef ASM_GEN_H
#define ASM_GEN_H

#include "iloc_ir.h"
#include "symbol_table.h"

/**
 * @brief Converts a register ID to its assembly representation.
 *
 * Special registers like the frame pointer (rbp) and static base (rbss)
 * are mapped to their respective names. Other registers are formatted as
 * RIP-relative virtual registers (e.g., rN(%rip)).
 *
 * @param reg_id   Register identifier.
 * @param buf      Buffer to store the resulting string.
 * @param bufsize  Size of the buffer.
 * @return         Pointer to the buffer containing the formatted register string.
 *
 * @note The buffer must be large enough to hold the output string.
 */
const char* asm_reg_to_str(int reg_id, char* buf, size_t bufsize);

/**
 * @brief Prints assembly directives for a single global variable.
 *
 * @param name Pointer to the name of the global variable.
 *
 * @details
 * Prints assembly directives to declare and allocate space for the global
 * variable. The output is printed directly to standard output and follows
 * the GCC-compatible syntax using the .bss section for uninitialized data.
 */
void print_asm_global_var(const char* name);

/**
 * @brief Translates a single ILOC operation into assembly code.
 *
 * @param op                Pointer to the ILOC operation to be translated.
 * @param globals_by_offset Array of global variable names indexed by their offset.
 *
 * @details
 * This function emits the equivalent x86_64 assembly instructions
 * corresponding to the given ILOC instruction. The `globals_by_offset`
 * array is used to resolve global variable names from their offsets
 * when generating memory access instructions.
 */
void print_iloc_to_asm(iloc_op_t* op, char** globals_by_offset);

/**
 * @brief Prints assembly code translated from ILOC intermediate representation.
 *
 * @param global_scope Pointer to the global symbol table containing
 *                     variable and function declarations.
 * @param iloc_code    Pointer to the list of ILOC operations to translate.
 *
 * @details
 * Traverses the ILOC operation list and prints corresponding assembly
 * instructions to standard output.
 */
void print_asm(symbol_table_t* global_scope, iloc_op_list_t* iloc_code);

#endif  // ASM_GEN_H
