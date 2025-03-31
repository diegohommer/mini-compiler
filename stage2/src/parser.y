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
       | /* empty */;


// PROGRAM LIST - A list of comma-separated elements
prog_list: element;
         | prog_list ',' element;


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
func_params: TK_PR_WITH param_def_list
           | /* empty */;

// PARAMETER LIST - A list of comma-separated parameter definitions
param_def_list: param_def
              | param_def_list ',' param_def;

// PARAMETER - Defines a single parameter with its type
param_def: TK_ID TK_PR_AS type


// FUNCTION BODY - A block of commands
func_body: cmd_block;



// ===========================
//     SIMPLE COMMANDS
// =========================== 

// SIMPLE COMMAND - Can be any of the following commands
simple_cmd: cmd_block
          | var_decl var_init
          | atribution
          | func_call
          | return_cmd
          | if_cmd else_cmd
          | while_cmd;


// COMMAND BLOCK - A optional [] delimited sequence of simple commands
cmd_block: '[' cmd_list ']'
         | '[' ']';

cmd_list: simple_cmd
        | cmd_list simple_cmd;


// VARIABLE DECLARATION - Declares a variable with a specific type
var_decl: TK_PR_DECLARE TK_ID TK_PR_AS type

// VARIABLE INITIALIZATION - Optionally initializes a variable with literal
var_init: TK_PR_WITH literal
        | /* empty */;


// ATRIBUTION - Defines a atribution
atribution: TK_ID TK_PR_IS expr;


// FUNCTION CALL - Calls the function with TK_ID name with call_args
func_call: TK_ID call_args;

// CALL ARGUMENTS - A optional () delimited list of comma-separated arguments
call_args: '(' call_args_list ')'
         | '(' ')';

call_args_list: expr
              | call_args_list ',' expr;


// RETURN COMMAND - Defines return statement with an expression and its type.
return_cmd: TK_PR_RETURN expr TK_PR_AS type;


// CONDITIONAL - Defines an if-else structure (optional else)
if_cmd: TK_PR_IF '(' expr ')' cmd_block;

else_cmd: TK_PR_ELSE cmd_block
        | /* empty */;


// REPETITION - Defines a while-loop structure
while_cmd: TK_PR_WHILE '(' expr ')' cmd_block;






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
