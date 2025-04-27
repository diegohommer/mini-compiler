%{ 
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <asd.h>

  int yylex(void);
  void yyerror (char const *mensagem);
  int get_line_number(void);
  extern asd_tree_t *tree;

  enum TokenType {
      IDENTIFIER = 1,
      LITERAL = 2
  };
%}

%union {
  char* str;
  asd_tree_t* tree;

  struct {
    int line;
    int type;
    char* value;
  } lexical_value;
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

%type <tree> def_func func_header func_params func_body  var_decl var_init
%type <tree> prog_list element cmd_block cmd_list call_args call_args_list
%type <tree> simple_cmd atribution func_call return_cmd if_else_cmd else_cmd while_cmd
%type <tree> exp n7 n6 n5 n4 n3 n2 n1 n0 prec0_ops
%type <str> prec5_ops prec4_ops prec3_ops prec2_ops prec1_ops

%define parse.error verbose
%start program

%%

// ===========================
//     PROGRAM STRUCTURE
// =========================== 

// PROGRAM - A optional list of elements followed by a semicolon
program: prog_list ';' { tree = $1; };
       | %empty { tree = NULL; };


// PROGRAM LIST - A list of comma-separated elements
prog_list: element { $$ = $1; }
         | prog_list ',' element { asd_add_child($$, $3); $$ = $1; };


// ELEMENT - Either a function definition or variable declaration
element: def_func { $$ = $1; }
       | var_decl { $$ = $1; };



// ===========================
//     FUNCTION STRUCTURE
// =========================== 

// FUNCTION DEFINITION - A header, optinal list of params and a body.
def_func: func_header func_params TK_PR_IS func_body { $$ = $1; asd_add_child($$, $4); };


// FUNCTION HEADER - Defines the function name and return type.
func_header: TK_ID TK_PR_RETURNS type { $$ = asd_new($1.value); };


// FUNCTION PARAMETERS - Defines an optional parameter list
func_params: TK_PR_WITH param_def_list { $$ = NULL; }
           | %empty { $$ = NULL; };

// PARAMETER LIST - A list of comma-separated parameter definitions
param_def_list: param_def
              | param_def_list ',' param_def;

// PARAMETER - Defines a single parameter with its type
param_def: TK_ID TK_PR_AS type;


// FUNCTION BODY - A block of commands
func_body: cmd_block { $$ = $1; };



// ===========================
//     SIMPLE COMMANDS
// =========================== 

// SIMPLE COMMAND - Can be any of the following commands
simple_cmd: cmd_block { $$ = $1; }
          | var_decl { $$ = $1; } 
          | atribution { $$ = $1; }
          | func_call { $$ = $1; }
          | return_cmd { $$ = $1; }
          | if_else_cmd { $$ = $1; }
          | while_cmd { $$ = $1; };


// COMMAND BLOCK - A optional [] delimited sequence of simple commands
cmd_block: '[' cmd_list ']' { $$ = $2; }
         | '[' ']' { $$ = NULL; };

cmd_list: simple_cmd { $$ = $1; }
        | cmd_list simple_cmd { asd_add_child($$, $2); $$ = $1; };


// VARIABLE DECLARATION - Declares a variable with a specific type
var_decl: TK_PR_DECLARE TK_ID TK_PR_AS type { $$ = NULL; };
        | TK_PR_DECLARE TK_ID TK_PR_AS type var_init { 
            $$ = asd_new("with");
            asd_add_child($$, asd_new($2.value)); 
            asd_add_child($$, $5); 
          }

// VARIABLE INITIALIZATION - Optionally initializes a variable with literal
var_init: TK_PR_WITH TK_LI_INT { $$ = asd_new($2.value); }
        | TK_PR_WITH TK_LI_FLOAT { $$ = asd_new($2.value); };


// ATRIBUTION - Defines an atribution
atribution: TK_ID TK_PR_IS exp { 
  $$ = asd_new("is"); 
  asd_add_child($$, asd_new($1.value)); 
  asd_add_child($$, $3); 
};


// FUNCTION CALL - Calls the function with TK_ID name with call_args
func_call: TK_ID call_args {
  // Build "call <function_id>" string
  char *buffer = malloc(strlen("call ") + strlen($1.value) + 1);
  strcpy(buffer, "call ");
  strcat(buffer, $1.value);
  $$ = asd_new(buffer);
  asd_add_child($$, $2); 
  free(buffer);  
};

// CALL ARGUMENTS - A optional () delimited list of comma-separated arguments
call_args: '(' call_args_list ')' { $$ = $2; }
         | '(' ')' { $$ = NULL; };

call_args_list: exp { $$ = $1; }
              | call_args_list ',' exp { asd_add_child($$, $3); $$ = $1; };


// RETURN COMMAND - Defines return statement with an expression and its type.
return_cmd: TK_PR_RETURN exp TK_PR_AS type { 
  $$ = asd_new("return"); 
  asd_add_child($$, $2); 
};


// CONDITIONAL - Defines an if-else structure (optional else)
if_else_cmd: TK_PR_IF '(' exp ')' cmd_block else_cmd {
  $$ = asd_new("if"); 
  asd_add_child($$, $3); 
  asd_add_child($$, $5);
  if($6 != NULL) 
    asd_add_child($$, $6);
};

else_cmd: TK_PR_ELSE cmd_block { $$ = $2; }
        | %empty { $$ = NULL; };


// REPETITION - Defines a while-loop structure
while_cmd: TK_PR_WHILE '(' exp ')' cmd_block { 
  $$ = asd_new("while"); 
  asd_add_child($$, $3); 
  asd_add_child($$, $5); 
};



// ===========================
//        EXPRESSIONS
// =========================== 

// EXPRESSION START
exp: n7 { $$ = $1; };


// PRECEDENCE 7 (LOWEST) - Bitwise OR
n7: n7 '|' n6 { $$ = asd_new("|"); asd_add_child($$, $1); asd_add_child($$, $3); }
  | n6 { $$ = $1; };


// PRECEDENCE 6 - Bitwise AND
n6: n6 '&' n5 { $$ = asd_new("&"); asd_add_child($$, $1); asd_add_child($$, $3); }
  | n5 { $$ = $1; };


// PRECEDENCE 5 - Comparison (==, !=)
prec5_ops: TK_OC_EQ { $$ = "=="; } 
         | TK_OC_NE { $$ = "!="; };

n5: n5 prec5_ops n4 { $$ = asd_new($2); asd_add_child($$, $1); asd_add_child($$, $3); }
  | n4 { $$ = $1; };


// PRECEDENCE 4 - Comparison (<, >, <=, >=)
prec4_ops: '<' { $$ = "<"; } 
         | '>' { $$ = ">"; } 
         | TK_OC_LE { $$ = "<="; } 
         | TK_OC_GE { $$ = ">="; };

n4: n4 prec4_ops n3 { $$ = asd_new($2); asd_add_child($$, $1); asd_add_child($$, $3); }
  | n3 { $$ = $1; };   


// PRECEDENCE 3 - Addition & Subtraction (+, -)
prec3_ops: '+' { $$ = "+"; } 
         | '-' { $$ = "-"; };

n3: n3 prec3_ops n2 { $$ = asd_new($2); asd_add_child($$, $1); asd_add_child($$, $3); }
  | n2 { $$ = $1; };


// PRECEDENCE 2 - Multiplication, Division, Modulo (*, /, %)
prec2_ops: '*' { $$ = "*"; } 
         | '/' { $$ = "/"; } 
         | '%' { $$ = "%"; };

n2: n2 prec2_ops n1 { $$ = asd_new($2); asd_add_child($$, $1); asd_add_child($$, $3); }
  | n1 { $$ = $1; };


// PRECEDENCE 1 - Unary Operators (+, -, !)
prec1_ops: '+' { $$ = "+"; } 
         | '-' { $$ = "-"; } 
         | '!' { $$ = "!"; };

n1: prec1_ops n1 { $$ = asd_new($1); asd_add_child($$, $2); }
  | n0 { $$ = $1; };


// PRECEDENCE 0 (HIGHEST) - FuncCalls, Ids, Literals, () delimited exps. 
prec0_ops: func_call { $$ = $1; } 
         | TK_ID { $$ = asd_new($1.value); } 
         | TK_LI_INT { $$ = asd_new($1.value); }
         | TK_LI_FLOAT { $$ = asd_new($1.value); }
         | '(' exp ')'{ $$ = $2; };

n0: prec0_ops { $$ = $1; };

// TYPE TOKENS
type: TK_PR_INT | TK_PR_FLOAT;

%%

void yyerror(const char *message) {
    fprintf(stderr, "Syntax error at line %d: \"%s\"\n", get_line_number(), message);
}