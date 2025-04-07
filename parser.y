%{
/* 
Definição do parser para realização da análise sintática.
Desenvolvido por:
  Diego Hommerding Amorim - 00341793
  Gabriel Gabriel Kenji Yatsuda Ikuta - 00337491
*/

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
atribution: TK_ID TK_PR_IS exp;


// FUNCTION CALL - Calls the function with TK_ID name with call_args
func_call: TK_ID call_args;

// CALL ARGUMENTS - A optional () delimited list of comma-separated arguments
call_args: '(' call_args_list ')'
         | '(' ')';

call_args_list: exp
              | call_args_list ',' exp;


// RETURN COMMAND - Defines return statement with an expression and its type.
return_cmd: TK_PR_RETURN exp TK_PR_AS type;


// CONDITIONAL - Defines an if-else structure (optional else)
if_cmd: TK_PR_IF '(' exp ')' cmd_block;

else_cmd: TK_PR_ELSE cmd_block
        | /* empty */;


// REPETITION - Defines a while-loop structure
while_cmd: TK_PR_WHILE '(' exp ')' cmd_block;



// ===========================
//        EXPRESSIONS
// =========================== 

// EXPRESSION START
exp: n7;

// PRECEDENCE 7 (LOWEST) - Bitwise OR
n7: n7 '|' n6
  | n6;

// PRECEDENCE 6 - Bitwise AND
n6: n6 '&' n5
  | n5;

// PRECEDENCE 5 - Comparison (==, !=)
prec5_ops: TK_OC_EQ | TK_OC_NE;
n5: n5 prec5_ops n4
  | n4;

// PRECEDENCE 4 - Comparison (<, >, <=, >=)
prec4_ops: '<' | '>' | TK_OC_LE | TK_OC_GE;
n4: n4 prec4_ops n3
  | n3;

// PRECEDENCE 3 - Addition & Subtraction (+, -)
prec3_ops: '+' | '-';
n3: n3 prec3_ops n2
  | n2;

// PRECEDENCE 2 - Multiplication, Division, Modulo (*, /, %)
prec2_ops: '*' | '/' | '%';
n2: n2 prec2_ops n1
  | n1;

// PRECEDENCE 1 - Unary Operators (+, -, !)
prec1_ops: '+' | '-' | '!';
n1: prec1_ops n1
  | n0;

// PRECEDENCE 0 (HIGHEST) - FuncCalls, Ids, Literals, () delimited exps. 
prec0_ops: func_call | TK_ID | literal | '(' exp ')';
n0: prec0_ops;

// LITERAL AND TYPE TOKENS
literal: TK_LI_INT | TK_LI_FLOAT;
type: TK_PR_INT | TK_PR_FLOAT;

%%

void yyerror (char const *mensagem) {
    printf("In line %d, there was an error:  \"%s\"\n", get_line_number(), mensagem);
};
