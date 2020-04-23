#include <stdio.h>
#include "structnodes.h"
#include "y.tab.h"

//pass the value of switch variable 
// types of variables
int ex(nodeType *p) {
    if (!p) return 0;
    switch(p->type) {
    case typeCon:       
        switch(p->con.type){
            case typeInt:    return p->con.iValue; 
            case typeFloat:  return p->con.fValue; 
            case typeString:  return p->con.sValue; 
            case typeChar:  return p->con.cValue; 
            case typeBool:  return p->con.bValue; 
        }
        
    case typeId:        return sym[p->id.idx];
    case typeOpr:
        switch(p->opr.oper) {
        case WHILE:     while(ex(p->opr.op[0])) ex(p->opr.op[1]); return 0;
        case IF:        if (ex(p->opr.op[0]))
                            ex(p->opr.op[1]);
                        else if (p->opr.nops > 2)
                            ex(p->opr.op[2]);
                        return 0;
        case PRINT:     printf("%d\n", ex(p->opr.op[0])); return 0; //for types of variables //modify
        case ';':       ex(p->opr.op[0]); return ex(p->opr.op[1]);
        case '=':       return sym[p->opr.op[0]->id.idx] = ex(p->opr.op[1]);
        case UMINUS:    return -ex(p->opr.op[0]);
        case '+':       return ex(p->opr.op[0]) + ex(p->opr.op[1]);
        case '-':       return ex(p->opr.op[0]) - ex(p->opr.op[1]);
        case '*':       return ex(p->opr.op[0]) * ex(p->opr.op[1]);
        case '/':       return ex(p->opr.op[0]) / ex(p->opr.op[1]);
        case '%':       return ex(p->opr.op[0]) / ex(p->opr.op[1]);
        case AND:       return ex(p->opr.op[0]) && ex(p->opr.op[1]);
        case OR:        return ex(p->opr.op[0]) || ex(p->opr.op[1]);
        case '!':       return !ex(p->opr.op[0]);
        case '<':       return ex(p->opr.op[0]) < ex(p->opr.op[1]);
        case '>':       return ex(p->opr.op[0]) > ex(p->opr.op[1]);
        case LE:        return ex(p->opr.op[0]) <= ex(p->opr.op[1]);
        case GE:        return ex(p->opr.op[0]) >= ex(p->opr.op[1]);
        case EQ:        return ex(p->opr.op[0]) == ex(p->opr.op[1]);
        case NE:        return ex(p->opr.op[0]) != ex(p->opr.op[1]);
        case FOR:       for(sym[p->opr.op[0]->id.idx] = ex(p->opr.op[1]); ex(p->opr.op[2]); sym[p->opr.op[3]->id.idx] = ex(p->opr.op[4])) ex(p->opr.op[5]); return 0;
        case REPEAT:  	do{
		                    ex(p->opr.op[0]);
                        }while (ex(p->opr.op[1]));
                    
        //case SWITCH:    ex(p->opr.op[1], ex(sym[p->opr.op[0]->id.idx]));

        //case CASE:      if(p->opr.op[0] != NULL) ex([p->opr.op[0]);
                        //if(switchval == ex(p->opr.op[1])  return ex(sym[p->opr.op[2]); //add indication of done; //modify
        
        //case DEFAULT:   if() ex([p->opr.op[1]); //!indication of done

        }
    }
    return 0;
}
 