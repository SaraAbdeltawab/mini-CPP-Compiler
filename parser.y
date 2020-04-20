/* definitions */
%{
    #include <stdio.h>
    #include <stdarg.h>
    #include "structnodes.h"

    /* prototypes */
    nodeType *opr(int oper, int nops, ...);
    nodeType *id(int i);
    nodeType *con();
    nodeType *conInt(int value);
    nodeType *conFloat(float value);
    nodeType *conBool(bool value);
    nodeType *conChar(char value);
    nodeType *conString(char value);
    void yyerror(char *);
    int yylex(void);

    int sym[26];
%}

%union {
    int   iValue;               /* integer value */	
    float fValue;               /* float value */
    char* sValue;               /* string value */
    char  cValue;               /* char value */
    bool  bValue;               /* bool value */
    char  sIndex;               /* symbol table index */
    nodeType *nPtr;             /* node pointer */
    
};

// Data
%token <iValue> INTEGER
%token <fValue> FLOAT
%token <cValue> CHAR
%token <sValue> STRING
%token <bValue> VAL_TRUE
%token <bValue> VAL_FALSE                    
%token <sIndex> VARIABLE
%token TYPE_INT TYPE_FLT TYPE_STR TYPE_CHR TYPE_BOOL TYPE_CONST EXIT  // Data types
%token IF ELSE WHILE FOR SWITCH CASE DEFAULT DO BREAK REPEAT UNTIL PRINT        // Keywords

%right '='
%left OR
%left AND
%left EQ NE
%left '<' '>' LE GE
%left '+' '-'
%left '*' '/' '%'
%right '!'
%nonassoc UNIMUS

%type <nPtr> expr

%%
program:
        program expr '\n'       { printf("%d\n", $2); }
        ;
            
// stmt :
//         ';'
//         | expr ';'
//         | VARIABLE '=' expr ';'
//         | 

expr:     
            INTEGER
        |   FLOAT                   
        |   CHAR                    
        |   STRING                  
        |   VAL_TRUE                
        |   VAL_FALSE               
        |   VARIABLE                { $$ = sym[$1] }
        |   '-' expr %prec UNIMUS   { $$ = $2 }
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

nodeType *con(){
    nodeType *p;

    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeCon;

    return p;
}

nodeType *conInt(int value) {
   nodeType *p = con();
    
    p->con.iValue = value;

    return p;
}

nodeType *conFloat(float value) {
    nodeType *p = con();

    p->con.fValue = value;

    return p;
}

nodeType *conBool(bool value) {
    nodeType *p = con();

    p->con.bValue = value;

    return p;
}


nodeType *conChar(char value) {
    nodeType *p = con();

    p->con.cValue = value;

    return p;
}

nodeType *conString(char* value) {
    nodeType *p = con();

    p->con.sValue = value; /* make sure that we don't need to copy it first //modify */

    return p;
}


nodeType *id(int i) {
    nodeType *p;

    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeId;
    p->id.i = i;

    return p;
}

nodeType *opr(int oper, int nops, ...) {
    va_list ap;
    nodeType *p;
    int i;

    /* allocate node, extending op array */
    if ((p = malloc(sizeof(nodeType) + (nops-1) * sizeof(nodeType *))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeOpr;
    p->opr.oper = oper;
    p->opr.nops = nops;
    va_start(ap, nops);
    for (i = 0; i < nops; i++)
        p->opr.op[i] = va_arg(ap, nodeType*); //why is the type ptrs //modify
    va_end(ap);
    return p;
}

void yyerror(char *s) {
    fprintf(stdout, "%s\n", s);
}

int main(void) {
    yyparse();
    return 0;
}