/*
Estrutura e declaração de funções para a construção e manipulação da Árvore Sintática Abstrata (AST).
Desenvolvido por:
  Professor Lucas Mello Schnorr
Usado por:
  Diego Hommerding Amorim - 00341793
  Gabriel Gabriel Kenji Yatsuda Ikuta - 00337491
*/

#ifndef _ASD_H_
#define _ASD_H_

typedef struct asd_tree
{
  char *label;
  int number_of_children;
  struct asd_tree **children;
} asd_tree_t;

/*
 * Function asd_new, creates a node without children with the given label.
 */
asd_tree_t *asd_new(const char *label);

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

#endif //_ASD_H_
