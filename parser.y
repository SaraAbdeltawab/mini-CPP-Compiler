/* definitions */
%{
    #include <stdio.h>
    #include <stdarg.h>
    #include <string.h>
    #include <stdlib.h>
    #include "structnodes.h"

    /* prototypes */
    nodeType *opr(int oper, int nops, ...);
    nodeType *id(char* name);
    nodeType *typ(conEnum value);
    nodeType *con();
    nodeType *conInt(int value);
    nodeType *conFloat(float value);
    nodeType *conBool(bool value);
    nodeType *conChar(char value);
    nodeType *conString(char* value);
    void freeNode(nodeType *p);
    void yyerror(char *);
    int yylex(void);
    int execute(nodeType *p);
    extern FILE* yyin;    /* defined by lex; lex reads from this file   */
    extern FILE* yyout;

    int yylineno;
%}

%union {
    float fValue;               /* float value */
    char* sValue;               /* string value */
    char*  sIndex;               /* symbol table index */
    nodeType *nPtr;             /* node pointer */
    
};

// Data
%token <fValue> INTEGER
%token <fValue> FLOAT
%token <fValue> CHAR
%token <sValue> STRING
%token <fValue> VAL_TRUE
%token <fValue> VAL_FALSE                    
%token <sIndex> VARIABLE
%token TYPE_INT TYPE_FLT TYPE_STR TYPE_CHR TYPE_BOOL TYPE_CONST EXIT  // Data types
%token IF ELSE WHILE FOR SWITCH CASE DEFAULT BREAK REPEAT UNTIL PRINT        // Keywords

%nonassoc IFX
%nonassoc ELSE

%right '='
%left OR
%left AND
%left EQ NE
%left '<' '>' LE GE
%left '+' '-'
%left '*' '/' '%'
%right '!'
%nonassoc UMINUS

%type <nPtr> stmt expr stmt_list case_default case_list type

%%

program:
        program stmt        {execute($2); freeNode($2);}
        | /* NULL */            
        ;

stmt:
            ';'                                                                   { $$ = opr(';', 2, NULL, NULL); }
        |   expr ';'                                                              { $$ = $1; }
        |   type VARIABLE ';'                                                     { $$ = opr('_',2,$1,id($2)); }
        |   VARIABLE '=' expr ';'                                                 { $$ = opr('=',2,id($1),$3); }
        |   type VARIABLE '=' expr ';'                                            { $$ = opr('=',3,$1,id($2),$4); }
        |   TYPE_CONST type VARIABLE '=' expr ';'                                 { $$ = opr('=',4,typ(conVar),$2,id($3),$5); }
        |   PRINT expr ';'                                                        { $$ = opr(PRINT,1,$2); }
        |   BREAK ';'                                                             { $$ = opr(BREAK,0); }
        |   SWITCH '(' VARIABLE ')' '{' case_list case_default '}'                { $$ = opr(SWITCH,3,id($3),$6,$7); }
        |   IF '(' expr ')' stmt %prec IFX                                        { $$ = opr(IF,2,$3,$5); }
        |   IF '(' expr ')' stmt ELSE stmt                                        { $$ = opr(IF,3,$3,$5,$7); }
        |   FOR '(' VARIABLE '=' expr ';' expr ';' VARIABLE '=' expr ')' stmt     { $$ = opr(FOR,6,id($3),$5,$7,id($9),$11,$13); }
        |   REPEAT stmt  UNTIL '(' expr ')' ';'                                   { $$ = opr(REPEAT,2,$2,$5); }
        |   WHILE '(' expr ')' stmt                                               { $$ = opr(WHILE,2,$3,$5); }
        |   '{' stmt_list '}'                                                     { $$ = $2; }
        |   error ';'                                                             { $$ = NULL; }
        |   error '}'                                                             { $$ = NULL; }
        ;

type:
            TYPE_INT           { $$ =  typ(typeInt); } 
        |   TYPE_FLT           { $$ =  typ(typeFloat); }
        |   TYPE_STR           { $$ =  typ(typeString); }
        |   TYPE_CHR           { $$ =  typ(typeChar); }
        |   TYPE_BOOL          { $$ =  typ(typeBool); }
        ;

case_default:
            DEFAULT ':' stmt_list                                               { $$ = opr(DEFAULT,1,$3); }
        |   /* NULL */                                                          { $$ = NULL; }
        ;

case_list:
            case_list CASE INTEGER ':' stmt_list                                { $$ = opr(CASE,3,$1,conInt($3),$5); }
        |   case_list CASE CHAR ':' stmt_list                                   { $$ = opr(CASE,3,$1,conChar($3),$5); }
        |   case_list CASE VAL_FALSE ':' stmt_list                              { $$ = opr(CASE,3,$1,conBool($3),$5);  }
        |   case_list CASE VAL_TRUE ':' stmt_list                               { $$ = opr(CASE,3,$1,conBool($3),$5); }
        |  /* NULL */                                                           { $$ = NULL; }
        ;

stmt_list:
            stmt                    { $$ = $1; }
        |   stmt_list stmt          { $$ = opr(';',2,$1,$2); }
        ;
        
expr:     
            INTEGER                 { $$ = conInt($1); }
        |   FLOAT                   { $$ = conFloat($1); }
        |   CHAR                    { $$ = conChar($1); }
        |   STRING                  { $$ = conString($1); }
        |   VAL_TRUE                { $$ = conBool($1); }
        |   VAL_FALSE               { $$ = conBool($1); }
        |   VARIABLE                { $$ = id($1); }
        |   '-' expr %prec UMINUS   { $$ = opr(UMINUS, 1, $2); }
        |   expr '+' expr           { $$ = opr('+', 2, $1, $3); }
        |   expr '-' expr           { $$ = opr('-', 2, $1, $3); }
        |   expr '*' expr           { $$ = opr('*', 2, $1, $3); }
        |   expr '/' expr           { $$ = opr('/', 2, $1, $3); }
        |   expr '%' expr           { $$ = opr('%', 2, $1, $3); }
        |   expr AND expr           { $$ = opr(AND, 2, $1, $3); }
        |   expr OR expr            { $$ = opr(OR, 2, $1, $3); }
        |   '!' expr                { $$ = opr('!', 1, $2); }
        |   expr '>' expr           { $$ = opr('>', 2, $1, $3); }
        |   expr '<' expr           { $$ = opr('<', 2, $1, $3); }
        |   expr LE expr            { $$ = opr(LE, 2, $1, $3); }
        |   expr GE expr            { $$ = opr(GE, 2, $1, $3); }
        |   expr EQ expr            { $$ = opr(EQ, 2, $1, $3); }
        |   expr NE expr            { $$ = opr(NE, 2, $1, $3); }
        |   '(' expr ')'            { $$ = $2; }       
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
    
    p->con.type = typeInt;
    p->con.fValue = value;
    
    return p;
}

nodeType *conFloat(float value) {
    nodeType *p = con();

    p->con.type = typeFloat;
    p->con.fValue = value;

    return p;
}

nodeType *conBool(bool value) {
    nodeType *p = con();

    p->con.type = typeBool;
    p->con.fValue = value;

    return p;
}


nodeType *conChar(char value) {
    nodeType *p = con();

    p->con.type = typeChar;
    p->con.fValue = value;

    return p;
}

nodeType *conString(char* value) {
    nodeType *p = con();


    p->con.type = typeString;
    p->con.sValue = value;


    return p;
}


nodeType *id(char* name) {
    nodeType *p;

    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeId;

    p->id.name = name;


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
        p->opr.op[i] = va_arg(ap, nodeType*);
    va_end(ap);
    return p;
}

nodeType *typ(conEnum type){
    nodeType *p;

    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeDef;
    p->typ.type = type;

    return p;
}

void freeNode(nodeType *p) {
    int i;

    if (!p) return;
    if (p->type == typeOpr) {
        for (i = 0; i < p->opr.nops; i++)
            freeNode(p->opr.op[i]);
    }
    free (p);
}

void yyerror(char *s) {
    fprintf(yyout, "line %d: %s\n", yylineno, s);
    exit(1);
}

int main(int argc, char* argv[]) { 

    if(argc != 3){
        printf("Wrong Arguments, Usage prog.exe inputfile outputfile\n");
        exit(0);
    }

    FILE* inputFile;
    FILE* outputFile;

    if((inputFile = fopen(argv[1], "r")) == NULL){
        printf("Error reading input file\n");
        exit(0);
    }
    if((outputFile = fopen(argv[2], "w")) == NULL){
        printf("Error reading input file\n");
        exit(0);
    }

    yyin = inputFile;
    yyout = outputFile;

    // while(feof(yyin)==0)
    // {
    //     yyparse();
    // }

    yyparse();
    fclose(yyin);
    fclose(yyout);

    return 0;
}