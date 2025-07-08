/*
 * Developed by: Diego Hommerding Amorim - 00341793
 *           Gabriel Kenji Yatsuda Ikuta - 00337491
 */

#ifndef ASM_GEN_H
#define ASM_GEN_H

#include "symbol_table.h"
#include "iloc_ir.h"

/**
 * @brief Generates assembly code for a single global variable.
 *
 * @param var              Pointer to the symbol representing the global variable.
 *
 * @details
 * This function outputs the assembly directives and initialization
 * code needed to declare and allocate space for the global variable
 * in the assembly output.
 */
void asm_gen_global_var(symbol_t* var);

/**
 * @brief Generates assembly code from ILOC intermediate representation.
 *
 * @param global_scope     Pointer to the global symbol table containing
 *                         variable and function declarations.
 * @param iloc_code        Pointer to the list of ILOC operations to be
 *                         translated into assembly.
 *
 * @details
 * This function traverses the provided ILOC operation list and uses the
 * information from the global symbol table to generate corresponding
 * assembly instructions. The generated assembly is printed directly to
 * the standard output.
 */
void asm_gen(symbol_table_t* global_scope, iloc_op_list_t* iloc_code);

#endif  // ASM_GEN_H

