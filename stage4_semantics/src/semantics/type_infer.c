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

type_t infer_return_type(scope_stack_t* scope_stack, asd_tree_t* return_expr, type_t declared_type)
{
    if (return_expr->data_type != declared_type) {
        display_assignment_type_error(return_expr->lexical_payload->line, return_expr->label,
                                      declared_type, return_expr->data_type);
        CLEAN_EXIT(scope_stack, ERR_WRONG_TYPE);
    }

    symbol_t* declared_function = scope_get_current_function(scope_stack);
    if (declared_function->type != declared_type) {
        display_return_type_error(
            declared_function->lex_value->line, return_expr->lexical_payload->line,
            declared_function->lex_value->value, declared_function->type, declared_type);
        CLEAN_EXIT(scope_stack, ERR_WRONG_TYPE);
    }

    return declared_type;
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