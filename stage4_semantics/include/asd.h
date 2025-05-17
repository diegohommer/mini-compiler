#ifndef _ASD_H_
#define _ASD_H_

#include <stdio.h>
#include <stdlib.h>

/**
 * @enum type_t
 * @brief Represents the type of a symbol (e.g., INT or FLOAT).
 */
typedef enum { INT = 0, FLOAT = 1 } type_t;

/**
 * @enum kind_t
 * @brief Represents the kind of a symbol in the abstract syntax tree (AST).
 */
typedef enum { IDENTIFIER = 1, LITERAL = 2, FUNCTION = 3 } kind_t;

/**
 * @struct lexical_value
 * @brief Represents the lexical information of a token.
 */
typedef struct lexical_value {
    int line;    /**< Line number in the source code. */
    int type;    /**< Token kind/type (IDENTIFIER or LITERAL). */
    char *value; /**< Lexeme (string value). */
} lexical_value_t;

/**
 * @struct asd_tree
 * @brief Represents a node in the abstract syntax tree (AST).
 */
typedef struct asd_tree {
    char *label;                      /**< Label for the node. */
    int data_type;                    /**< Data type of the node (INT or FLOAT) */
    int number_of_children;           /**< Number of child nodes. */
    struct asd_tree **children;       /**< Array of pointers to child nodes. */
    lexical_value_t *lexical_payload; /**< Lexical value associated with this node, if any. */
} asd_tree_t;

/**
 * @brief Creates a new AST node with no children.
 *
 * Copies the label and the payload (if not NULL). The caller may free the original payload after
 * this call.
 *
 * @param label Label for the node.
 * @param data_type The data type (e.g., INT or FLOAT) associated with this node.
 * @param payload Lexical payload (optional).
 * @return Pointer to a new AST node.
 */
asd_tree_t *asd_new(const char *label, type_t data_type, lexical_value_t *payload);

/**
 * @brief Frees the AST node and its children.
 *
 * Also frees the copied label and payload.
 *
 * @param tree AST root node to free.
 */
void asd_free(asd_tree_t *tree);

/**
 * @brief Adds a child to the given AST node.
 *
 * @param tree Pointer to the parent node.
 * @param child Pointer to the child node to add.
 */
void asd_add_child(asd_tree_t *tree, asd_tree_t *child);

/**
 * @brief Recursively prints the AST to standard output.
 *
 * Prints the tree structure in a human-readable, indented format.
 *
 * @param tree Pointer to the AST root node to print.
 */
void asd_print(asd_tree_t *tree);

/**
 * @brief Prints the AST in DOT (Graphviz) format.
 *
 * Can be used to generate visualizations with Graphviz tools.
 *
 * @param tree Pointer to the AST root node.
 */
void asd_print_graphviz(asd_tree_t *tree);

/**
 * @brief Prints the AST in DOT format with debug info.
 *
 * Includes lexical line and type information in each node.
 *
 * @param tree Pointer to the AST root node.
 */
void asd_debug_graphviz(asd_tree_t *tree);

#endif  // _ASD_H_
