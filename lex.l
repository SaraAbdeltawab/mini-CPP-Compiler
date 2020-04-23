digit [0-9]
letter [a-zA-Z]
smallletter [a-z]

%{
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "structnodes.h"
#include "y.tab.h"

#define MAX_STRING_SZ 100
void yyerror(char *);
char buf[MAX_STRING_SZ];
char *s;
%}
%x STR

%%


"if"      return IF;
"else"    return ELSE;
"while"	  return WHILE;
"for"     return FOR;
"switch"  return SWITCH;
"case"    return CASE;
"default" return DEFAULT;
"break"   return BREAK;
"repeat"  return REPEAT;
"until"   return UNTIL;
"print"   return PRINT;


"&&"		return AND;
"||"		return OR;
"=="		return EQ;
"!="		return NE;
">="		return GE;
"<="		return LE;



"int"			return TYPE_INT;
"float"			return TYPE_FLT;
"string"  		return TYPE_STR;
"char"			return TYPE_CHR;
"bool"          return TYPE_BOOL;
"const"   		return TYPE_CONST;
"exit"			return EXIT;

"true"          {
                    yylval.bValue = true;
                    return VAL_TRUE; 
                }

"false"         {
                    yylval.bValue = false;
                    return VAL_FALSE;
                }


\"           { BEGIN STR; s = buf; }
<STR>\\n     { *s++ = '\n'; }
<STR>\\t     { *s++ = '\t'; }
<STR>\\\"    { *s++ = '\"'; }
<STR>\"      {
                    *s = '\0';
                    yylval.sValue = malloc(strlen(buf));
                    strcpy(yylval.sValue, buf); 
                    BEGIN INITIAL;
                    //printf("found '%s'\n", buf);
                    return STRING;
                }
<STR>\n      { printf("unterminated string constant"); exit(1); }
<STR>.       { *s++ = *yytext; }

{smallletter}     { 
                    yylval.sIndex = *yytext - 'a';
                    return VARIABLE;
                }

0           {
                yylval.iValue = atoi(yytext);
                return INTEGER;
            }

[1-9]{digit}* {
                yylval.iValue = atoi(yytext);
                return INTEGER;
            }

{digit}+.{digit}+ {
                    yylval.fValue = atof(yytext);
                    return FLOAT;
              }

\"{letter}\"      {
                    yylval.cValue = *yytext;
                    return CHAR;
                }


    
[-!()<>=+*/;{}.] {   //where "." is used?
                   return *yytext;
                }


[ \t\n]+        ;       /* ignore whitespace */

.              {
                   yyerror("Unknown character");
                } 
%%

int yywrap(void) {
    return 1;
}

