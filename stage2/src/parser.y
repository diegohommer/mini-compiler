%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror (char const *mensagem);
int get_line_number(void);
%}

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
%token TK_ID
%token TK_LI_INT
%token TK_LI_FLOAT
%token TK_ER

%define parse.error verbose
%start program

%%

// ===========================
//     PROGRAM STRUCTURE
// =========================== 

// PROGRAM - A optional list of elements followed by a semicolon
program: prog_list ';';
program: ;

// PROGRAM LIST - A list of comma-separated elements
prog_list: element;
prog_list: prog_list ',' element;

// ELEMENT - Either a function definition or variable declaration
element: def_func | var_decl;



// ===========================
//     FUNCTION STRUCTURE
// =========================== 

// FUNCTION DEFINITION - A header, optinal list of params and a body.
def_func: func_header func_params TK_PR_IS func_body;

// FUNCTION HEADER - Defines the function name and return type.
func_header: TK_ID TK_PR_RETURNS type;

// FUNCTION PARAMETERS - Defines an optional parameter list
func_params: TK_PR_WITH param_def_list;
func_params: ;

// PARAMETER LIST - A list of comma-separated defined parameters
param_def_list: param
param_def_list: param_def_list ',' param

// PARAMETER - Defines a single parameter with its type
param: TK_ID TK_PR_AS type

// FUNCTION BODY - A block of commands
func_body: cmd_block;



// ===========================
//     SIMPLE COMMANDS
// =========================== 

// SIMPLE COMMAND - Can be any of the following commands
simple_cmd: cmd_block;
simple_cmd: var_decl var_init;
simple_cmd: atr;
simple_cmd: func_call;

// COMMAND BLOCK - A optional [] delimited list of simple commands
cmd_block: '[' cmd_list ']';
cmd_block: '[' ']';
cmd_list: simple_cmd;
cmd_list: cmd_list simple_cmd;

// VARIABLE DECLARATION - Declares a variable with a specific type
var_decl: TK_PR_DECLARE TK_ID TK_PR_AS type

// VARIABLE INITIALIZATION - Optionally initializes a variable with literal
var_init: ;
var_init: TK_PR_WITH literal;

// ATRIBUTION - 
atr: TK_ID TK_PR_IS expr;

// FUNCTION CALL - 
func_call: TK_ID call_args;

// CALL ARGUMENTS -
call_args: '(' call_args_list ')';
call_args: '(' ')';

// CALL ARGUMENTS LIST -
call_args_list: expr;
call_args_list: call_args_list ',' expr;


// ===========================
//        EXPRESSIONS
// =========================== 


expr: TK_LI_INT;

literal: TK_LI_INT | TK_LI_FLOAT;
type: TK_PR_INT | TK_PR_FLOAT;






// exp: n7;
// n7: n7 '|' n6;
// n7: n6;

%%

void yyerror (char const *mensagem) {
    printf("Na linha %d, houve o erro  \"%s\"\n", get_line_number(), mensagem);
};
