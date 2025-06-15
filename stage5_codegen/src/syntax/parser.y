%code requires {
    /*
     * Developed by: Diego Hommerding Amorim - 00341793
     *               Gabriel Kenji Yatsuda Ikuta - 00337491
     */
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #include "type_infer.h"
    #include "iloc_gen.h"

    typedef struct arg {
        int count;
        asd_tree_t* args;
    } arg_t;
}

%code {
    int yylex(void);
    void yyerror(char const *mensagem);

    asd_tree_t* make_unary_exp_tree(const char* op_token, asd_tree_t* operand);
    asd_tree_t* make_exp_tree(asd_tree_t* left, const char* op_token, asd_tree_t* right);
    asd_tree_t* build_list(asd_tree_t* head, asd_tree_t* tail);
    arg_t* create_arg(asd_tree_t* args, int count);
    type_t get_token_type(lexical_value_t* lexical_value);
    void free_lex_value(lexical_value_t* lexical_value);

    extern int get_line_number(void);
    extern asd_tree_t* tree;
    extern scope_stack_t* scope_stack;
}

%union {
    type_t type;
    asd_tree_t* tree;
    lexical_value_t* lexical_value;
    arg_t* args;
}

%destructor {
    if ($$ != NULL && $$ != tree) {
        asd_free($$);
    }
} <tree>;

/* Tokens */
%token TK_PR_AS
%token TK_PR_DECLARE
%token TK_PR_ELSE
%token TK_PR_FLOAT
%token TK_PR_IF
%token TK_PR_INT
%token TK_PR_IS
%token TK_PR_RETURN
%token TK_PR_RETURNS
%token TK_PR_WHILE
%token TK_PR_WITH
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token <lexical_value> TK_ID
%token <lexical_value> TK_LI_INT
%token <lexical_value> TK_LI_FLOAT
%token TK_ER

/* Non-terminals with types */
%type <tree> def_func func_header func_body var_decl var_init
%type <tree> prog_list element cmd_block func_cmd_block cmd_list
%type <tree> simple_cmd atribution func_call return_cmd if_else_cmd else_cmd while_cmd
%type <tree> exp n7 n6 n5 n4 n3 n2 n1 n0 literal
%type <args> call_args call_args_list
%type <type> type

%define parse.error verbose
%start program

%%

/* ===========================
 *     PROGRAM STRUCTURE
 * =========================== */

/* PROGRAM - A optional list of elements followed by a semicolon */
program
    : create_scope prog_list destroy_scope ';'  { tree = $2; }
    | %empty                                    { tree = NULL; }
    ;

/* PROGRAM LIST - A list of comma-separated elements */
prog_list
    : element                { $$ = $1; }
    | element ',' prog_list  { $$ = build_list($1, $3); }
    ;

/* ELEMENT - Either a function definition or variable declaration */
element
    : def_func  { $$ = $1; }
    | var_decl  { $$ = $1; }
    ;



/* ===========================
 *     FUNCTION STRUCTURE
 * =========================== */

/* FUNCTION DEFINITION - A header, optinal list of params and a body */
def_func
    : func_header create_scope func_params TK_PR_IS func_body destroy_scope
        {
            $$ = $1;
            if($5 != NULL) {
                asd_add_child($$, $5);
                iloc_op_list_concat($$->code, $5->code);
            }
        }
    ;

/* FUNCTION HEADER - Defines the function name and return type */
func_header
    : TK_ID TK_PR_RETURNS type
        {
            $$ = asd_new($1->value, $3, $1, 0);
            scope_declare_symbol(scope_stack, symbol_new(FUNCTION, $3, $1));
            free_lex_value($1);
        }
    ;

/* FUNCTION PARAMETERS - Defines an optional parameter list */
func_params
    : TK_PR_WITH param_def_list
    | %empty
    ;

/* PARAMETER LIST - A list of comma-separated parameter definitions */
param_def_list
    : param_def
    | param_def ',' param_def_list
    ;

/* PARAMETER - Defines a single parameter with its type */
param_def
    : TK_ID TK_PR_AS type
        {
            scope_declare_function_parameter(scope_stack, symbol_new(IDENTIFIER, $3, $1));
            free_lex_value($1); 
        }
    ;

/* FUNCTION BODY - A block of commands */
func_body
    : func_cmd_block  { $$ = $1; }
    ;



/* ===========================
 *      SIMPLE COMMANDS
 * =========================== */

/* SIMPLE COMMAND - Can be any of the following commands */
simple_cmd
    : cmd_block    { $$ = $1; }
    | var_decl     { $$ = $1; }
    | var_init     { $$ = $1; }
    | atribution   { $$ = $1; }
    | func_call    { $$ = $1; }
    | return_cmd   { $$ = $1; }
    | if_else_cmd  { $$ = $1; }
    | while_cmd    { $$ = $1; }
    ;


/* COMMAND BLOCK - An optional [] delimited sequence of simple commands */
cmd_block
    : '[' create_scope cmd_list destroy_scope ']'  { $$ = $3; }
    | '[' ']'                                      { $$ = NULL; }
    ;

func_cmd_block
    : '[' cmd_list ']'  { $$ = $2; }
    | '[' ']'           { $$ = NULL; }
    ;

cmd_list
    : simple_cmd           { $$ = $1; }
    | simple_cmd cmd_list  { $$ = build_list($1, $2); }
    ;


/* VARIABLE DECLARATION - Declares a variable with a specific type */
var_decl
    : TK_PR_DECLARE TK_ID TK_PR_AS type
        {
            $$ = NULL;
            scope_declare_symbol(scope_stack, symbol_new(IDENTIFIER, $4, $2));
            free_lex_value($2);
        }
    ;


/* VARIABLE INITIALIZATION - Declares and initializes a variable with literal */
var_init
    : TK_PR_DECLARE TK_ID TK_PR_AS type TK_PR_WITH literal
        {
            type_t var_type = infer_initialization_type(scope_stack, $2, $4, $6->data_type);
            symbol_t* var = scope_declare_symbol(scope_stack, symbol_new(IDENTIFIER, var_type, $2));

            $$ = asd_new("with", var_type, $2, 2, asd_new($2->value, var_type, $2, 0), $6);

            iloc_gen_store($$, $6, var->offset, scope_stack->num_tables);
            free_lex_value($2);
        }
    ;


/* ATRIBUTION - Defines an atribution */
atribution
    : TK_ID TK_PR_IS exp
        {
            symbol_t* decl_var = scope_get_symbol(scope_stack, $1->value, $1->line);
            type_t var_type = infer_atribution_type(scope_stack, $1, decl_var, $3->data_type);
            $$ = asd_new("is", var_type, $1, 2, asd_new($1->value, var_type, $1, 0), $3);

            iloc_gen_store($$, $3, decl_var->offset, scope_stack->num_tables);
            free_lex_value($1);
        }
    ;


/* FUNCTION CALL - Calls the function with TK_ID name with call_args */
func_call
    : TK_ID call_args
        {
            int call_type = infer_function_call_type(scope_stack, $1, $2->args, $2->count);

            int len = strlen("call ") + strlen($1->value) + 1;
            char *buffer = malloc(len);
            snprintf(buffer, len, "call %s", $1->value);

            $$ = asd_new(buffer, call_type, $1, 1, $2->args);

            free($2);
            free(buffer);
            free_lex_value($1);
        }
    ;

/* CALL ARGUMENTS - A optional () delimited list of comma-separated arguments. */
call_args
    : '(' call_args_list ')'  { $$ = $2; }
    | '(' ')'                 { $$ = create_arg(NULL, 0); }
    ;

call_args_list
    : exp  { $$ = create_arg($1, 1); }
    | exp ',' call_args_list 
        {
            asd_add_child($1, $3->args);
            $$ = create_arg($1, 1 + $3->count);
            free($3);
        }
    ;


/* RETURN COMMAND - Defines return statement with an expression and its type */
return_cmd
    : TK_PR_RETURN exp TK_PR_AS type
        {
            int return_type = infer_return_type(scope_stack, $2, $4);
            $$ = asd_new("return", return_type, $2->lexical_payload, 1, $2);
        }
    ;


/* CONDITIONAL - Defines an if-else structure (optional else) */
if_else_cmd
    : TK_PR_IF '(' exp ')' cmd_block else_cmd
        {
            int if_type = infer_if_type(scope_stack, $3->data_type, $5, $6);
            $$ = asd_new("if", if_type, $3->lexical_payload, 3, $3, $5, $6);
        }
    ;

else_cmd
    : TK_PR_ELSE cmd_block  { $$ = $2; }
    | %empty                { $$ = NULL; }
    ;


/* REPETITION - Defines a while-loop structure */
while_cmd
    : TK_PR_WHILE '(' exp ')' cmd_block
        { $$ = asd_new("while", $3->data_type, $3->lexical_payload, 2, $3, $5); }
    ;



/* ===========================
 *          EXPRESSIONS
 * =========================== */

/* EXPRESSION START */
exp
    : n7  { $$ = $1; }
    ;

/* PRECEDENCE 7 (LOWEST) - Bitwise OR */
n7
    : n7 '|' n6  { $$ = make_exp_tree($1, "|", $3); }
    | n6         { $$ = $1; }
    ;

/* PRECEDENCE 6 - Bitwise AND */
n6
    : n6 '&' n5  { $$ = make_exp_tree($1, "&", $3); }
    | n5         { $$ = $1; }
    ;

/* PRECEDENCE 5 - Comparison (==, !=) */
n5
    : n5 TK_OC_EQ n4  { $$ = make_exp_tree($1, "==", $3); }
    | n5 TK_OC_NE n4  { $$ = make_exp_tree($1, "!=", $3); }
    | n4              { $$ = $1; }
    ;

/* PRECEDENCE 4 - Comparison (<, >, <=, >=) */
n4
    : n4 '<' n3       { $$ = make_exp_tree($1, "<", $3); }
    | n4 '>' n3       { $$ = make_exp_tree($1, ">", $3); }
    | n4 TK_OC_LE n3  { $$ = make_exp_tree($1, "<=", $3); }
    | n4 TK_OC_GE n3  { $$ = make_exp_tree($1, ">=", $3); }
    | n3              { $$ = $1; }
    ;

/* PRECEDENCE 3 - Addition & Subtraction (+, -) */
n3
    : n3 '+' n2  { $$ = make_exp_tree($1, "+", $3); }
    | n3 '-' n2  { $$ = make_exp_tree($1, "-", $3); }
    | n2         { $$ = $1; }
    ;

/* PRECEDENCE 2 - Multiplication, Division, Modulo (*, /, %) */
n2
    : n2 '*' n1  { $$ = make_exp_tree($1, "*", $3); }
    | n2 '/' n1  { $$ = make_exp_tree($1, "/", $3); }
    | n2 '%' n1  { $$ = make_exp_tree($1, "%", $3); }
    | n1         { $$ = $1; }
    ;

/* PRECEDENCE 1 - Unary Operators (+, -, !) */
n1
    : '+' n1  { $$ = make_unary_exp_tree("+", $2); }
    | '-' n1  { $$ = make_unary_exp_tree("-", $2); }
    | '!' n1  { $$ = make_unary_exp_tree("!", $2); }
    | n0      { $$ = $1; }
    ;

/* PRECEDENCE 0 (HIGHEST) - FuncCalls, Ids, Literals, Parenthesized expressions */
n0
    : func_call    { $$ = $1; }
    | literal      { $$ = $1; }
    | '(' exp ')'  { $$ = $2; }
    | TK_ID
        {
            int var_type = infer_var_type(scope_stack, $1);
            $$ = asd_new($1->value, var_type, $1, 0);
            free_lex_value($1);
        }
    ;


/* TYPE AND LITERAL TOKENS */
type
    : TK_PR_INT    { $$ = INT; }
    | TK_PR_FLOAT  { $$ = FLOAT; }
    ;

literal
    : TK_LI_INT
        {
            $$ = asd_new($1->value, INT, $1, 0);
            iloc_gen_literal($$, $1->value);
            free_lex_value($1);
        }
    | TK_LI_FLOAT
        {
            $$ = asd_new($1->value, FLOAT, $1, 0);
            free_lex_value($1);
        }
    ;



/* ===========================
 *       SYMBOL TABLES
 * =========================== */

/* SCOPE NON-TERMINALS - For creating and destroying symbol tables on a given scope */
create_scope
    : %empty  { scope_push(scope_stack); }
    ;

destroy_scope
    : %empty  { scope_pop(scope_stack); }
    ;
%%



/* Error reporting function */
void yyerror(const char *message)
{
    fprintf(stderr, "Syntax error at line %d: \"%s\"\n", get_line_number(), message);
}

/* Create AST node for unary expressions */
asd_tree_t* make_unary_exp_tree(const char* op_token, asd_tree_t* operand)
{
    asd_tree_t* tree = asd_new(op_token, operand->data_type, operand->lexical_payload, 1, operand);
    iloc_gen_unary_exp(tree, op_token, operand);
    return tree;
}

/* Create AST node for binary expressions */
asd_tree_t* make_exp_tree(asd_tree_t* left, const char* op_token, asd_tree_t* right)
{
    type_t exp_type = infer_exp_type(scope_stack, op_token, left, right);
    asd_tree_t* tree = asd_new(op_token, exp_type, left->lexical_payload, 2, left, right);

    iloc_gen_binary_exp(tree, left, op_token, right);

    return tree;
}

/* Build list of AST nodes, concatenating code lists */
asd_tree_t* build_list(asd_tree_t* head, asd_tree_t* tail)
{
    if (head == NULL) {
        return tail;
    } else {
        if (tail != NULL) {
            asd_add_child(head, tail);
            iloc_op_list_concat(head->code, tail->code);
        }
        return head;
    }
}

/* Create argument structure for function arguments */
arg_t* create_arg(asd_tree_t* args, int count)
{
    arg_t* arg = malloc(sizeof(arg_t));
    arg->count = count;
    arg->args = args;
    return arg;
}

/* Free lexical value data */
void free_lex_value(lexical_value_t* lexical_value)
{
    free(lexical_value->value);
    free(lexical_value);
}

