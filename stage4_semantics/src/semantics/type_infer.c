#include "type_infer.h"

type_t infer_exp_type(scope_stack_t* scope_stack, const char* op, asd_tree_t* tree_left,
                      asd_tree_t* tree_right)
{
    if (tree_left->data_type != tree_right->data_type) {
        display_expression_type_error(tree_left->lexical_payload->line, op, tree_left->data_type,
                                      tree_right->data_type);
        CLEAN_EXIT(scope_stack, ERR_WRONG_TYPE);
    }
    return tree_left->data_type;
}

type_t infer_if_type(scope_stack_t* scope_stack, type_t cond_type, asd_tree_t* if_block,
                     asd_tree_t* else_block)
{
    if (else_block != NULL) {
        if (if_block->data_type != else_block->data_type) {
            display_if_else_type_error(if_block->lexical_payload->line, if_block->data_type,
                                      else_block->data_type);
            CLEAN_EXIT(scope_stack, ERR_WRONG_TYPE);
        }
    }
    return cond_type;
}