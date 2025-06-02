/*
 * Developed by: Diego Hommerding Amorim - 00341793
 *           Gabriel Kenji Yatsuda Ikuta - 00337491
 */

#ifndef _ASD_H_
#define _ASD_H_

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @enum type_t
 * @brief Represents the data type of a symbol or AST node.
 */
typedef enum { INT = 0, FLOAT = 1 } type_t;

/**
 * @enum kind_t
 * @brief Represents the kind of symbol in the abstract syntax tree (AST).
 */
typedef enum { IDENTIFIER = 0, LITERAL = 1, FUNCTION = 2 } kind_t;

/**
 * @struct lexical_value_t
 * @brief Stores lexical information for a token.
 */
typedef struct lexical_value {
    int line;    /**< Source code line number */
    kind_t type; /**< Token kind (IDENTIFIER or LITERAL) */
    char *value; /**< Lexeme string */
} lexical_value_t;

/**
 * @struct asd_tree_t
 * @brief Represents a node in the abstract syntax tree (AST).
 */
typedef struct asd_tree {
    char *label;                      /**< Node label */
    type_t data_type;                 /**< Data type (INT or FLOAT) */
    int number_of_children;           /**< Number of child nodes */
    struct asd_tree **children;       /**< Array of pointers to child nodes */
    lexical_value_t *lexical_payload; /**< Associated lexical information, if any */
} asd_tree_t;

/**
 * @brief Creates a new AST node with the specified children.
 *
 * Copies the label and lexical payload (if provided).
 * Accepts a variable number of child nodes.
 *
 * @param label Node label.
 * @param data_type Data type of the node.
 * @param payload Optional lexical payload (may be NULL).
 * @param num_children Number of child nodes.
 * @param ... Variable number of pointers to child nodes (asd_tree_t*).
 * @return Pointer to the newly allocated AST node, or NULL on allocation failure.
 */
asd_tree_t *asd_new(const char *label, type_t data_type, lexical_value_t *payload, int num_children,
                    ...);

/**
 * @brief Frees an AST node and all its descendants.
 *
 * Also frees the copied label and lexical payload.
 *
 * @param tree Pointer to the root node to free.
 */
void asd_free(asd_tree_t *tree);

/**
 * @brief Adds a child node to a given AST node.
 *
 * @param tree Pointer to the parent node.
 * @param child Pointer to the child node to add.
 */
void asd_add_child(asd_tree_t *tree, asd_tree_t *child);

/**
 * @brief Recursively prints the AST in a readable, indented format.
 *
 * @param tree Pointer to the root AST node.
 */
void asd_print(asd_tree_t *tree);

/**
 * @brief Prints the AST in DOT format for Graphviz visualization.
 *
 * @param tree Pointer to the root AST node.
 */
void asd_print_graphviz(asd_tree_t *tree);

/**
 * @brief Prints the AST in DOT format with lexical debug information.
 *
 * Includes line numbers and token types in node labels.
 *
 * @param tree Pointer to the root AST node.
 */
void asd_debug_graphviz(asd_tree_t *tree);

#endif  // _ASD_H_
