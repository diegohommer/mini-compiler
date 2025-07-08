/*
 * Developed by: Diego Hommerding Amorim - 00341793
 *           Gabriel Kenji Yatsuda Ikuta - 00337491
 */

#ifndef ASM_GEN_H
#define ASM_GEN_H

#include "iloc_ir.h"
#include "symbol_table.h"

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
