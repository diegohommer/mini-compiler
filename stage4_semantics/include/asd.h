#ifndef _ASD_H_
#define _ASD_H_

#include <stdio.h>
#include <stdlib.h>

/**
 * @enum type_t
 * @brief Represents the data type of a symbol or AST node.
 */
typedef enum { INT = 0, FLOAT = 1 } type_t;

/**
 * @enum kind_t
 * @brief Represents the kind of symbol in the abstract syntax tree (AST).
 */
typedef enum { IDENTIFIER = 1, LITERAL = 2, FUNCTION = 3 } kind_t;

/**
 * @struct lexical_value_t
 * @brief Stores lexical information for a token.
 */
typedef struct lexical_value {
    int line;    /**< Source code line number */
    int type;    /**< Token kind/type (IDENTIFIER or LITERAL) */
    char *value; /**< Lexeme string */
} lexical_value_t;

/**
 * @struct asd_tree_t
 * @brief Represents a node in the abstract syntax tree (AST).
 */
typedef struct asd_tree {
    char *label;                      /**< Node label */
    int data_type;                    /**< Data type (INT or FLOAT) */
    int number_of_children;           /**< Number of child nodes */
    struct asd_tree **children;       /**< Array of pointers to child nodes */
    lexical_value_t *lexical_payload; /**< Associated lexical information, if any */
} asd_tree_t;

/**
 * @brief Creates a new AST node with no children.
 *
 * Copies the label and lexical payload (if provided).
 *
 * @param label Node label.
 * @param data_type Data type of the node.
 * @param payload Optional lexical payload (may be NULL).
 * @return Pointer to the newly allocated AST node.
 */
asd_tree_t *asd_new(const char *label, type_t data_type, lexical_value_t *payload);

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
