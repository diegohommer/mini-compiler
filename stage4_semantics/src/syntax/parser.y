%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <stdarg.h>
  #include <stddef.h>

  #include "scope_stack.h"

  int yylex(void);
  void yyerror (char const *mensagem);
  asd_tree_t* make_tree(const char* label, lexical_value_t* val, int num_children, ...);
  asd_tree_t* build_list(asd_tree_t* head, asd_tree_t* tail);
  void free_lex_value(lexical_value_t* lexical_value);
  int get_line_number(void);
  extern asd_tree_t *tree;
  extern scope_stack_t *scope_stack;
%}

%union {
  char* str;
  int type;
  asd_tree_t* tree;
  lexical_value_t* lexical_value;
}

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

%type <tree> def_func func_header func_params func_body param_def param_def_list var_decl var_init
%type <tree> prog_list element cmd_block func_cmd_block cmd_list call_args call_args_list
%type <tree> simple_cmd atribution func_call return_cmd if_else_cmd else_cmd while_cmd
%type <tree> exp n7 n6 n5 n4 n3 n2 n1 n0 prec0_ops literal
%type <str> prec5_ops prec4_ops prec3_ops prec2_ops prec1_ops
%type <type> type

%define parse.error verbose
%start program

%%

// ===========================
//     PROGRAM STRUCTURE
// =========================== 

// PROGRAM - A optional list of elements followed by a semicolon
program: create_scope prog_list destroy_scope ';' { tree = $2; };
       | %empty { tree = NULL; };

// PROGRAM LIST - A list of comma-separated elements
prog_list: element { $$ = $1; }
         | element ',' prog_list { $$ = build_list($1, $3); };

// ELEMENT - Either a function definition or variable declaration
element: def_func { $$ = $1; }
       | var_decl { $$ = $1; };


// ===========================
//     FUNCTION STRUCTURE
// =========================== 

// FUNCTION DEFINITION - A header, optinal list of params and a body.
def_func: func_header create_scope func_params TK_PR_IS func_body destroy_scope {
  $$ = $1;
  if($5 != NULL) asd_add_child($$, $5);
};

// FUNCTION HEADER - Defines the function name and return type.
func_header: TK_ID TK_PR_RETURNS type { 
  $$ = asd_new($1->value, $1);
  scope_add_symbol(scope_stack, symbol_new(FUNCTION, $3, $1));
  free_lex_value($1);
};

// FUNCTION PARAMETERS - Defines an optional parameter list
func_params: TK_PR_WITH param_def_list { $$ = NULL; }
           | %empty { $$ = NULL; };

// PARAMETER LIST - A list of comma-separated parameter definitions
param_def_list: param_def { $$ = NULL; }
              | param_def ',' param_def_list { $$ = NULL; };

// PARAMETER - Defines a single parameter with its type
param_def: TK_ID TK_PR_AS type { free_lex_value($1); };

// FUNCTION BODY - A block of commands
func_body: func_cmd_block { $$ = $1; };


// ===========================
//     SIMPLE COMMANDS
// =========================== 

// SIMPLE COMMAND - Can be any of the following commands
simple_cmd: cmd_block { $$ = $1; }
          | var_decl { $$ = $1; }
          | var_init { $$ = $1; }
          | atribution { $$ = $1; }
          | func_call { $$ = $1; }
          | return_cmd { $$ = $1; }
          | if_else_cmd { $$ = $1; }
          | while_cmd { $$ = $1; };


// COMMAND BLOCK - A optional [] delimited sequence of simple commands
cmd_block: '[' create_scope cmd_list destroy_scope ']' { $$ = $3; }
         | '[' ']' { $$ = NULL; };

func_cmd_block: '[' cmd_list ']' { $$ = $2; }
         | '[' ']' { $$ = NULL; };

cmd_list: simple_cmd { $$ = $1; }
        | simple_cmd cmd_list { $$ = build_list($1, $2); };


// VARIABLE DECLARATION - Declares a variable with a specific type
var_decl: TK_PR_DECLARE TK_ID TK_PR_AS type { 
  $$ = NULL;  
  scope_add_symbol(scope_stack, symbol_new(IDENTIFIER, $4, $2));
  free_lex_value($2);
};


// VARIABLE INITIALIZATION - Declares and initializes a variable with literal
var_init: TK_PR_DECLARE TK_ID TK_PR_AS type TK_PR_WITH literal {
  $$ = make_tree("with", NULL, 2, asd_new($2->value, $2), $6);
  scope_add_symbol(scope_stack, symbol_new(IDENTIFIER, $4, $2));
  free_lex_value($2);
}


// ATRIBUTION - Defines an atribution
atribution: TK_ID TK_PR_IS exp {
  $$ = make_tree("is", NULL, 2, asd_new($1->value, $1), $3);
  free_lex_value($1);
};


// FUNCTION CALL - Calls the function with TK_ID name with call_args
func_call: TK_ID call_args {
  int len = strlen("call ") + strlen($1->value) + 1;
  char *buffer = malloc(len);
  snprintf(buffer, len, "call %s", $1->value);
  $$ = make_tree(buffer, $1, 1, $2);
  free_lex_value($1);
  free(buffer);
};

// CALL ARGUMENTS - A optional () delimited list of comma-separated arguments
call_args: '(' call_args_list ')' { $$ = $2; }
         | '(' ')' { $$ = NULL; };

call_args_list: exp { $$ = $1; }
              | exp ',' call_args_list { $$ = build_list($1, $3); };


// RETURN COMMAND - Defines return statement with an expression and its type.
return_cmd: TK_PR_RETURN exp TK_PR_AS type {
  $$ = make_tree("return", NULL, 1, $2);
};


// CONDITIONAL - Defines an if-else structure (optional else)
if_else_cmd: TK_PR_IF '(' exp ')' cmd_block else_cmd {
  $$ = make_tree("if", NULL, 3, $3, $5, $6);
};

else_cmd: TK_PR_ELSE cmd_block { $$ = $2; }
        | %empty { $$ = NULL; };


// REPETITION - Defines a while-loop structure
while_cmd: TK_PR_WHILE '(' exp ')' cmd_block {
  $$ = make_tree("while", NULL, 2, $3, $5);
}



// ===========================
//        EXPRESSIONS
// =========================== 

// EXPRESSION START
exp: n7 { $$ = $1; };

// PRECEDENCE 7 (LOWEST) - Bitwise OR
n7: n7 '|' n6 { $$ = make_tree("|", NULL, 2, $1, $3); }
  | n6 { $$ = $1; };

// PRECEDENCE 6 - Bitwise AND
n6: n6 '&' n5 { $$ = make_tree("&", NULL, 2, $1, $3); }
  | n5 { $$ = $1; };

// PRECEDENCE 5 - Comparison (==, !=)
prec5_ops: TK_OC_EQ { $$ = "=="; } 
         | TK_OC_NE { $$ = "!="; };
n5: n5 prec5_ops n4 { $$ = make_tree($2, NULL, 2, $1, $3); }
  | n4 { $$ = $1; };

// PRECEDENCE 4 - Comparison (<, >, <=, >=)
prec4_ops: '<' { $$ = "<"; } 
         | '>' { $$ = ">"; } 
         | TK_OC_LE { $$ = "<="; } 
         | TK_OC_GE { $$ = ">="; };
n4: n4 prec4_ops n3 { $$ = make_tree($2, NULL, 2, $1, $3); }
  | n3 { $$ = $1; };   


// PRECEDENCE 3 - Addition & Subtraction (+, -)
prec3_ops: '+' { $$ = "+"; } 
         | '-' { $$ = "-"; };
n3: n3 prec3_ops n2 { $$ = make_tree($2, NULL, 2, $1, $3); }
  | n2 { $$ = $1; };

// PRECEDENCE 2 - Multiplication, Division, Modulo (*, /, %)
prec2_ops: '*' { $$ = "*"; } 
         | '/' { $$ = "/"; } 
         | '%' { $$ = "%"; };
n2: n2 prec2_ops n1 { $$ = make_tree($2, NULL, 2, $1, $3); }
  | n1 { $$ = $1; };

// PRECEDENCE 1 - Unary Operators (+, -, !)
prec1_ops: '+' { $$ = "+"; } 
         | '-' { $$ = "-"; } 
         | '!' { $$ = "!"; };
n1: prec1_ops n1 { $$ = make_tree($1, NULL, 1, $2); }
  | n0 { $$ = $1; };

// PRECEDENCE 0 (HIGHEST) - FuncCalls, Ids, Literals, () delimited exps. 
prec0_ops: func_call { $$ = $1; } 
         | TK_ID { $$ = asd_new($1->value, $1); free_lex_value($1); } 
         | literal { $$ = $1; }
         | '(' exp ')'{ $$ = $2; };
n0: prec0_ops { $$ = $1; };

// TYPE AND LITERAL TOKENS
type: TK_PR_INT { $$ = INT; }
    | TK_PR_FLOAT { $$ = FLOAT; };
literal: TK_LI_INT { $$ = asd_new($1->value, $1); free_lex_value($1); }
       | TK_LI_FLOAT { $$ = asd_new($1->value, $1); free_lex_value($1); };



// ===========================
//       SYMBOL TABLES
// ===========================

// SCOPE NON-TERMINALS - For creating and destroying symbol tables on a given scope
create_scope: %empty { scope_push(scope_stack); };
destroy_scope: %empty { scope_stack_debug_print(scope_stack); scope_pop(scope_stack); };
%%

void yyerror(const char *message) {
  fprintf(stderr, "Syntax error at line %d: \"%s\"\n", get_line_number(), message);
}

asd_tree_t* make_tree(const char* label, lexical_value_t* val, int num_children, ...){
  asd_tree_t* parent = asd_new(label, val);

	va_list args;
	va_start(args, num_children);

	for (int i = 0; i < num_children; ++i) {
		asd_tree_t* child = va_arg(args, asd_tree_t*);
		if (child != NULL) {
			asd_add_child(parent, child);
		}
	}

	va_end(args);
	return parent;
}

asd_tree_t* build_list(asd_tree_t* head, asd_tree_t* tail){
  if(head == NULL){
    return tail;
  }else {
    if(tail != NULL) asd_add_child(head, tail);
    return head;
  }
}

void free_lex_value(lexical_value_t* lexical_value){
  free(lexical_value->value);
  free(lexical_value);
}