#include <stdbool.h>

typedef enum { typeCon, typeId, typeOpr, typeDef } nodeEnum;
typedef enum { typeInt, typeFloat, typeString, typeChar, typeBool, conVar } conEnum;

/* constants */
typedef struct {
    conEnum type;                   /* type of con */

    /* value of constant */
    union {
        float fValue;               /* float value */
        char* sValue;               /* string value */
    };
                    
} conNodeType; 

/* identifiers */
typedef struct {
    int idx;                      /* subscript to sym array */
} idNodeType;

/* operators */
typedef struct {
    int oper;                   /* operator */
    int nops;                   /* number of operands */
    struct nodeTypeTag *op[1];	/* operands, extended at runtime */
} oprNodeType;

typedef struct nodeTypeTag {
    nodeEnum type;              /* type of node */

    union {
        conNodeType con;        /* constants */
        idNodeType id;          /* identifiers */
        oprNodeType opr;        /* operators */
    };
} nodeType;

extern conNodeType sym[26];
