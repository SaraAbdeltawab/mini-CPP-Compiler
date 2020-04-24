#include <stdbool.h>

typedef enum { typeCon, typeId, typeOpr, typeDef } nodeEnum;
typedef enum { typeNotDefined, typeInt, typeFloat, typeString, typeChar, typeBool, conVar } conEnum;

/* constants */
struct conNodeType{
    conEnum type;                   /* type of con */

    /* value of constant */
    union {
        float fValue;               /* float value */
        char* sValue;               /* string value */
    };
                    
}; 

/* identifiers */
struct idNodeType{
    char name;                      
};

/* operators */
struct oprNodeType{
    int oper;                   /* operator */
    int nops;                   /* number of operands */
    struct nodeTypeTag *op[1];	/* operands, extended at runtime */
};

/* types */
struct typeNodeType{
    conEnum type;                     
};


typedef struct nodeTypeTag {
    nodeEnum type;              /* type of node */

    union {
        struct conNodeType con;        /* constants */
        struct idNodeType id;          /* identifiers */
        struct oprNodeType opr;        /* operators */
        struct typeNodeType typ;       /* types */
    };
} nodeType;
