%{
    #include "stdio.h"
    #include "parser.tab.h"
%}

%option noyywrap yylineno

%%
    /* IGNORE */
\/\/.*        // Ignore comments
[\n\r\t ]     // Ignore whitespaces

    /* RESERVED KEYWORDS */
as      return TK_PR_AS; 
declare return TK_PR_DECLARE;
else    return TK_PR_ELSE;
float   return TK_PR_FLOAT;
if      return TK_PR_IF;
int     return TK_PR_INT;
is      return TK_PR_IS;
return  return TK_PR_RETURN;
returns return TK_PR_RETURNS;
while   return TK_PR_WHILE;
with    return TK_PR_WITH;

    /* MONOCHAR OPERATORS & DELIMITERS */
[\!\*\/\%\+\-\<\>\[\]\(\)\=\,\;\&\|] return (int) yytext[0];

    /* MULTICHAR OPERATORS */
\<\=      return TK_OC_LE;
\>\=      return TK_OC_GE;
\=\=      return TK_OC_EQ;
\!\=      return TK_OC_NE;

    /* IDENTIFIERS AND LITERALS */
[a-z_][a-z0-9_]* return TK_ID;
[0-9]+  return TK_LI_INT;
[0-9]*\.[0-9]+ return TK_LI_FLOAT;

.   return TK_ER;
%%

int get_line_number(){
    return yylineno;
}
