/*
 * Developed by: Diego Hommerding Amorim - 00341793
 *           Gabriel Kenji Yatsuda Ikuta - 00337491
 */

#include "type_infer.h"

#include <stdlib.h>

#include "errors.h"
#include "symbol_table.h"

type_t infer_initialization_type(scope_stack_t* scope_stack, lexical_value_t* var_id,
                                 type_t decl_type, type_t exp_type)
{
    if (decl_type != exp_type) {
        display_initialization_type_error(var_id->line, var_id->value, decl_type, exp_type);
        CLEAN_EXIT(scope_stack, ERR_WRONG_TYPE);
    }

    return decl_type;
}

type_t infer_atribution_type(scope_stack_t* scope_stack, lexical_value_t* var_id,
                             symbol_t* var_decl, type_t exp_type)
{
    if (var_decl->kind != IDENTIFIER) {
        display_function_error(var_id->value, var_id->line, var_decl->lex_value->line);
        CLEAN_EXIT(scope_stack, ERR_FUNCTION);
    }

    if (var_decl->type != exp_type) {
        display_atribution_type_error(var_id->line, var_decl->lex_value->line, var_id->value,
                                      var_decl->type, exp_type);
        CLEAN_EXIT(scope_stack, ERR_WRONG_TYPE);
    }

    return exp_type;
}

type_t infer_function_call_type(scope_stack_t* scope_stack, lexical_value_t* call_id,
                                asd_tree_t* call_args, int num_args)
{
    symbol_t* function_symbol = scope_get_symbol(scope_stack, call_id->value, call_id->line);

    if (function_symbol->kind != FUNCTION) {
        display_variable_error(call_id->value, call_id->line, function_symbol->lex_value->line);
        CLEAN_EXIT(scope_stack, ERR_VARIABLE);
    }

    int num_expected_args = 0;
    if (function_symbol->params != NULL) {
        num_expected_args = function_symbol->params->num_parameters;
    }

    if (num_expected_args > num_args) {
        display_missing_args_error(call_id->line, function_symbol->lex_value->line, call_id->value,
                                   num_expected_args, num_args);
        CLEAN_EXIT(scope_stack, ERR_MISSING_ARGS);
    }

    if (num_expected_args < num_args) {
        display_excess_args_error(call_id->line, function_symbol->lex_value->line, call_id->value,
                                  num_expected_args, num_args);
        CLEAN_EXIT(scope_stack, ERR_EXCESS_ARGS);
    }

    int i;
    asd_tree_t* arg = call_args;
    // Validate each argument type
    for (i = 0; i < num_expected_args; i++) {
        type_t expected_type = function_symbol->params->parameters[i]->type;
        type_t provided_type = arg->data_type;
        if (expected_type != provided_type) {
            display_wrong_type_args_error(
                call_id->line, function_symbol->lex_value->line, call_id->value, i + 1,
                function_symbol->params->parameters[i]->label, expected_type, provided_type);
            CLEAN_EXIT(scope_stack, ERR_WRONG_TYPE_ARGS);
        }

        if (arg->number_of_children > 0) {
            arg = arg->children[arg->number_of_children - 1];
        } else {
            arg = NULL;
        }
    }

    return function_symbol->type;
}

type_t infer_return_type(scope_stack_t* scope_stack, asd_tree_t* return_expr, type_t declared_type)
{
    if (return_expr->data_type != declared_type) {
        display_atribution_type_error(return_expr->lexical_payload->line,
                                      return_expr->lexical_payload->line, return_expr->label,
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
    if (if_block != NULL && else_block != NULL) {
        if (if_block->data_type != else_block->data_type) {
            display_if_else_type_error(if_block->lexical_payload->line, if_block->data_type,
                                       else_block->data_type);
            CLEAN_EXIT(scope_stack, ERR_WRONG_TYPE);
        }
    }

    return cond_type;
}

type_t infer_exp_type(scope_stack_t* scope_stack, const char* op, asd_tree_t* tree_left,
                      asd_tree_t* tree_right)
{
    if (tree_left->data_type != tree_right->data_type) {
        display_expression_type_error(tree_right->lexical_payload->line, op, tree_left->data_type,
                                      tree_right->data_type);
        CLEAN_EXIT(scope_stack, ERR_WRONG_TYPE);
    }

    return tree_left->data_type;
}

type_t infer_var_type(scope_stack_t* scope_stack, lexical_value_t* var_id)
{
    symbol_t* var_decl = scope_get_symbol(scope_stack, var_id->value, var_id->line);

    if (var_decl->kind != IDENTIFIER) {
        display_function_error(var_id->value, var_id->line, var_decl->lex_value->line);
        CLEAN_EXIT(scope_stack, ERR_FUNCTION);
    }

    return var_decl->type;
}
