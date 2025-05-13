#ifndef _ASD_H_
#define _ASD_H_

typedef enum TokenType { IDENTIFIER = 1, LITERAL = 2 } type_t;

typedef struct lexical_value {
    int line;
    int type;
    char *value;
} lexical_value_t;

typedef struct asd_tree {
    char *label;
    int number_of_children;
    struct asd_tree **children;
    lexical_value_t *lexical_payload;
} asd_tree_t;

/*
 * Function asd_new, creates a node without children with the given label and
 * payload.
 */
asd_tree_t *asd_new(const char *label, lexical_value_t *payload);

/*
 * Function asd_free, recursively frees the node and its children.
 */
void asd_free(asd_tree_t *tree);

/*
 * Function asd_add_child, adds child as a child of tree.
 */
void asd_add_child(asd_tree_t *tree, asd_tree_t *child);

/*
 * Function asd_print, recursively prints the tree.
 */
void asd_print(asd_tree_t *tree);

/*
 * Function asd_print_graphviz, same as above, in DOT format.
 */
void asd_print_graphviz(asd_tree_t *tree);

/*
 * Function asd_debug_graphviz, like asd_print_graphviz but includes line and
 * type info.
 */
void asd_debug_graphviz(asd_tree_t *tree);

#endif  //_ASD_H_