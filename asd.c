/*
Implementação das funções para a construção e manipulação da Árvore Sintática
Abstrata (AST). Desenvolvido por: Professor Lucas Mello Schnorr Usado por: Diego
Hommerding Amorim - 00341793 Gabriel Gabriel Kenji Yatsuda Ikuta - 00337491
*/

#include "asd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

asd_tree_t *asd_new(const char *label, lexical_value_t *payload) {
  asd_tree_t *ret = NULL;
  ret = calloc(1, sizeof(asd_tree_t));
  if (ret != NULL) {
    ret->label = strdup(label);
    ret->number_of_children = 0;
    ret->children = NULL;
    ret->lexical_payload = payload;
  }
  return ret;
}

void asd_free(asd_tree_t *tree) {
  if (tree != NULL) {
    int i;
    for (i = 0; i < tree->number_of_children; i++) {
      asd_free(tree->children[i]);
    }
    free(tree->children);
    free(tree->label);
    if (tree->lexical_payload != NULL) {
      free(tree->lexical_payload->value);
    }
    free(tree->lexical_payload);
    free(tree);
  } else {
    printf("Erro: %s recebeu parâmetro tree = %p.\n", __FUNCTION__, tree);
  }
}

void asd_add_child(asd_tree_t *tree, asd_tree_t *child) {
  if (tree != NULL && child != NULL) {
    tree->number_of_children++;
    tree->children = realloc(tree->children,
                             tree->number_of_children * sizeof(asd_tree_t *));

    tree->children[tree->number_of_children - 1] = child;
  } else {
    printf("Erro: %s recebeu parâmetro tree = %p / %p.\n", __FUNCTION__, tree,
           child);
  }
}

static void _asd_print(FILE *foutput, asd_tree_t *tree, int profundidade) {
  int i;
  if (tree != NULL) {
    fprintf(foutput, "%d%*s: Nó '%s' tem %d filhos:\n", profundidade,
            profundidade * 2, "", tree->label, tree->number_of_children);
    for (i = 0; i < tree->number_of_children; i++) {
      _asd_print(foutput, tree->children[i], profundidade + 1);
    }
  } else {
    printf("Erro: %s recebeu parâmetro tree = %p.\n", __FUNCTION__, tree);
  }
}

void asd_print(asd_tree_t *tree) {
  FILE *foutput = stderr;
  if (tree != NULL) {
    _asd_print(foutput, tree, 0);
  } else {
    printf("Erro: %s recebeu parâmetro tree = %p.\n", __FUNCTION__, tree);
  }
}

static void _asd_print_graphviz(FILE *foutput, asd_tree_t *tree) {
  if (tree != NULL) {
    fprintf(foutput, "  %ld [ label=\"%s\" ];\n", (long)tree, tree->label);
    for (int i = 0; i < tree->number_of_children; i++) {
      fprintf(foutput, "  %ld -> %ld;\n", (long)tree, (long)tree->children[i]);
      _asd_print_graphviz(foutput, tree->children[i]);
    }
  } else {
    printf("Erro: %s recebeu parâmetro tree = %p.\n", __FUNCTION__, tree);
  }
}

static void _asd_debug_graphviz(FILE *foutput, asd_tree_t *tree) {
  if (tree != NULL) {
    if (tree->lexical_payload != NULL) {
      // Malloc necessary memory for custom string on label_with_info
      int needed_len =
          snprintf(NULL, 0, "%s\n (line=%d, type=%d)", tree->label,
                   tree->lexical_payload->line, tree->lexical_payload->type);

      char *label_with_info = malloc(needed_len + 1);
      if (!label_with_info) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
      }

      // Transfer that string to label with info
      snprintf(label_with_info, needed_len + 1, "%s\n (line=%d, type=%d)",
               tree->label, tree->lexical_payload->line,
               tree->lexical_payload->type);

      fprintf(foutput, "  %ld [ label=\"%s\" ];\n", (long)tree,
              label_with_info);
      free(label_with_info);
    } else {
      fprintf(foutput, "  %ld [ label=\"%s\" ];\n", (long)tree, tree->label);
    }

    for (int i = 0; i < tree->number_of_children; i++) {
      fprintf(foutput, "  %ld -> %ld;\n", (long)tree, (long)tree->children[i]);
      _asd_debug_graphviz(foutput, tree->children[i]);
    }
  }
}

void asd_print_graphviz(asd_tree_t *tree) {
  FILE *foutput = stdout;
  if (tree != NULL) {
    fprintf(foutput, "digraph grafo {\n");
    _asd_print_graphviz(foutput, tree);
    fprintf(foutput, "}\n");
  } else {
    printf("Erro: %s recebeu parâmetro tree = %p.\n", __FUNCTION__, tree);
  }
}

void asd_debug_graphviz(asd_tree_t *tree) {
  FILE *foutput = stdout;
  if (tree != NULL) {
    fprintf(foutput, "digraph grafo {\n");
    _asd_debug_graphviz(foutput, tree);
    fprintf(foutput, "}\n");
  } else {
    printf("Erro: %s recebeu parâmetro tree = %p.\n", __FUNCTION__, tree);
  }
}
