/* definitions */
%{
    #include <stdio.h>
    void yyerror(char *);
    int yylex(void);

    int sym[26];
%}

%token INTEGER VARIABLE FLOAT CHAR STRING VAL_TRUE VAL_FALSE                    // Data
%token TYPE_INT TYPE_LONG TYPE_FLT TYPE_STR TYPE_CHR TYPE_BOOL TYPE_CONST EXIT  // Data types
%token IF ELSE WHILE FOR SWITCH CASE DEFAULT DO BREAK REPEAT UNTIL PRINT        // Keywords

%right '='
%left OR
%left AND
%left EQ NE
%left '<' '>' LE GE
%left '+' '-'
%left '*' '/' '%'
%right '!'

%%

/* rules */
program:
            program expr '\n'       { printf("%d\n", $2); }

// stmt :
//         ';'
//         | expr ';'
//         | VARIABLE '=' expr ';'
//         | 

expr :     
            INTEGER
        |   FLOAT                   
        |   CHAR                    
        |   STRING                  
        |   VAL_TRUE                
        |   VAL_FALSE               
        |   VARIABLE                { $$ = sym[$1] }
        |   expr '+' expr           { $$ = $1 + $3 }
        |   expr '-' expr           { $$ = $1 - $3 }
        |   expr '*' expr           { $$ = $1 * $3 }
        |   expr '/' expr           { $$ = $1 / $3 }
        |   expr '%' expr           { $$ = $1 % $3 }
        |   expr AND expr           { $$ = $1 && $3 }
        |   expr OR expr            { $$ = $1 || $3 }
        |   '!' expr                { $$ = ! $2 }
        |   expr '>' expr           { $$ = $1 > $2 }
        |   expr '<' expr           { $$ = $1 < $2 }
        |   expr LE expr            { $$ = $1 <= $2 }
        |   expr GE expr            { $$ = $1 >= $2 }
        |   expr EQ expr            { $$ = $1 == $2 }
        |   expr NE expr            { $$ = $1 !=  $2 }
        |   '(' expr ')'            { $$ = $2 }       
        ;
%% 

/* routines */

void yyerror(char *s) {
    fprintf(stdout, "%s\n", s);
}

int main(void) {
    yyparse();
    return 0;
}