#include <stdio.h>
#include <stdarg.h>
#include "structnodes.h"
#include "y.tab.h"

bool skipflag = 0;
bool exSwitch(nodeType *p, int switchval, bool* casematch);

int ex(nodeType *p) {
    bool casematch = 0;
    if (!p) return 0;
    if(skipflag) return 0;
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
        case WHILE:     while(ex(p->opr.op[0])){
                            ex(p->opr.op[1]);
                            if(skipflag){
                                skipflag = 0;
                                break;
                            }
                        }return 0;
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
        case FOR:       for(sym[p->opr.op[0]->id.idx] = ex(p->opr.op[1]); ex(p->opr.op[2]); sym[p->opr.op[3]->id.idx] = ex(p->opr.op[4])){
                            ex(p->opr.op[5]);
                            if(skipflag){
                                skipflag = 0;
                                break;
                            }
                        } return 0;
        case REPEAT:  	do{
		                    ex(p->opr.op[0]);
                        }while (ex(p->opr.op[1])); return 0; //Is return needed here?//modify
                    
        case SWITCH:    if(!exSwitch(p->opr.op[1], sym[p->opr.op[0]->id.idx], &casematch)) ex(p->opr.op[2]); skipflag = 0; return 0;

        case DEFAULT:   ex(p->opr.op[0]); return 0;
        
        case BREAK:     skipflag = 1; return 0;
        }
    }
    return 0;
}
 

bool exSwitch(nodeType *p, int switchval, bool* casematch){
    if(p->opr.op[0] != NULL) exSwitch(p->opr.op[0], switchval, casematch);
    if(switchval == ex(p->opr.op[1]) && !*casematch) {
        *casematch = 1;
        ex(p->opr.op[2]);
    }
    else if(*casematch && !skipflag) ex(p->opr.op[2]);

    return *casematch;
}