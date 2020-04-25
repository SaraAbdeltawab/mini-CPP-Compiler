#include <stdio.h>
#include<stdlib.h>
#include <stdarg.h>
#include "symboltable.h"
#include "y.tab.h"

bool skipflag = 0;
bool exSwitch(nodeType *p, int switchval, bool* casematch);

struct conNodeType* ex(nodeType *p) {
    bool casematch = 0;
    struct conNodeType* pt = malloc(sizeof(struct conNodeType*));
    struct conNodeType* pt2;
    conEnum type;
    if (!p) return NULL;
    if(skipflag) return NULL;
    switch(p->type) {
    case typeCon:       
        pt->type = p->con.type;
        switch(p->con.type){
            case typeInt:        pt->fValue = p->con.fValue; pt->type=typeInt; 
            case typeFloat:      pt->fValue = p->con.fValue; pt->type=typeFloat; 
            case typeString:     pt->sValue = p->con.sValue; pt->type=typeString; 
            case typeChar:       pt->fValue = p->con.fValue; pt->type=typeChar; 
            case typeBool:       pt->fValue = p->con.fValue; pt->type=typeBool; 
        }
        return pt;

    case typeId:        return retrieve(p->id.name);
    case typeDef:       pt->type = p->typ.type; return pt;
    case typeOpr:
        switch(p->opr.oper) {
        case WHILE:     pt = ex(p->opr.op[0]);
                        if (pt->type == typeString){
                            while(pt->sValue){
                                ex(p->opr.op[1]);
                                if(skipflag){
                                    skipflag = 0;
                                    break;
                                }
                            }return NULL;
                        }else{
                            while(pt->fValue){
                                ex(p->opr.op[1]);
                                if(skipflag){
                                    skipflag = 0;
                                    break;
                                }
                            }return NULL;
                        }

        case IF:        pt = ex(p->opr.op[0]);
                        if (pt->type == typeString){
                            if (pt->sValue)
                                ex(p->opr.op[1]);
                            else if (p->opr.nops > 2)
                                ex(p->opr.op[2]);
                            return NULL;

                        }else{
                            if (pt->fValue)
                                ex(p->opr.op[1]);
                            else if (p->opr.nops > 2)
                                ex(p->opr.op[2]);
                            return NULL;
                        }

        case PRINT:     pt = ex(p->opr.op[0]);
                        switch (pt->type){
                            case typeInt:   printf("%d\n", (int)pt->fValue); break;

                            case typeFloat: printf("%f\n", pt->fValue); break;
                                
                            case typeChar:  printf("%c\n", (char)pt->fValue); break;
                            
                            case typeString:printf("%s\n", pt->sValue); break;

                            case typeBool:  printf("%d\n", (int)pt->fValue); break;
                        }
                        return NULL;
        
        case ';':       ex(p->opr.op[0]); return ex(p->opr.op[1]);

        case '_':       return insert(p->opr.op[1]->id.name, ex(p->opr.op[0])->type, *pt, 0, 0);

        case '=':       switch (p->opr.nops)
                        {
                            case 2: return insert(p->opr.op[0]->id.name, typeNotDefined,*ex(p->opr.op[1]), 0, 1);
                                
                            case 3: pt = ex(p->opr.op[0]);
                                    type =  pt->type;
                                    pt = ex(p->opr.op[2]);
                                    return insert(p->opr.op[1]->id.name, type, *pt, 0, 1);
                                
                            case 4: pt = ex(p->opr.op[1]);
                                    type =  pt->type;
                                    pt = ex(p->opr.op[3]);
                                    return insert(p->opr.op[2]->id.name, type, *pt, 1, 1);
                        }

        case UMINUS:    pt = ex(p->opr.op[0]);
                        if (pt->type == typeString || pt->type == typeChar ){
                            printf("Error: can't perform this operation on strings or char");
                            return NULL;
                        }else{
                            pt->fValue = -(pt->fValue);
                            return pt;
                        }

        case '+':       pt = ex(p->opr.op[0]); pt2 = ex(p->opr.op[1]);
                        if ((pt->type == typeString || pt->type == typeChar)
                            ||
                            (pt2->type == typeString || pt2->type == typeChar)
                        ){
                            printf("Error: can't perform this operation on strings or char");
                            return NULL;
                        }else{
                            pt->fValue = pt->fValue + pt2->fValue;
                            return pt;
                        }

        case '-':       pt = ex(p->opr.op[0]); pt2 = ex(p->opr.op[1]);
                        if ((pt->type == typeString || pt->type == typeChar)
                            ||
                            (pt2->type == typeString || pt2->type == typeChar)
                        ){
                            printf("Error: can't perform this operation on strings or char");
                            return NULL;
                        }else{
                            pt->fValue = pt->fValue - pt2->fValue;
                            return pt;
                        }

        case '*':       pt = ex(p->opr.op[0]); pt2 = ex(p->opr.op[1]);
                        if ((pt->type == typeString || pt->type == typeChar)
                            ||
                            (pt2->type == typeString || pt2->type == typeChar)
                        ){
                            printf("Error: can't perform this operation on strings or char");
                            return NULL;
                        }else{
                            pt->fValue = pt->fValue * pt2->fValue;
                            return pt;
                        }

        case '/':       pt = ex(p->opr.op[0]); pt2 = ex(p->opr.op[1]);
                        if ((pt->type == typeString || pt->type == typeChar)
                            ||
                            (pt2->type == typeString || pt2->type == typeChar)
                        ){
                            printf("Error: can't perform this operation on strings or char");
                            return NULL;
                        }else{
                            pt->fValue = pt->fValue / pt2->fValue;
                            return pt;
                        }

        case '%':       pt = ex(p->opr.op[0]); pt2 = ex(p->opr.op[1]);
                        if ((pt->type == typeString || pt->type == typeChar)
                            ||
                            (pt2->type == typeString || pt2->type == typeChar)
                        ){
                            printf("Error: can't perform this operation on strings or char");
                            return NULL;
                        }else{
                            pt->fValue = (int)(pt->fValue) % (int)(pt2->fValue);
                            return pt;
                        }
                        
        case AND:       pt->fValue = ex(p->opr.op[0])->fValue && ex(p->opr.op[1])->fValue; return pt;
        case OR:        pt->fValue = ex(p->opr.op[0])->fValue || ex(p->opr.op[1])->fValue; return pt;
        case '!':       pt->fValue = !ex(p->opr.op[0])->fValue; return pt;

        case '<':       pt->fValue = ex(p->opr.op[0])->fValue < ex(p->opr.op[1])->fValue; return pt;
        case '>':       pt->fValue = ex(p->opr.op[0])->fValue > ex(p->opr.op[1])->fValue; return pt;
        case LE:        pt->fValue = ex(p->opr.op[0])->fValue <= ex(p->opr.op[1])->fValue; return pt;
        case GE:        pt->fValue = ex(p->opr.op[0])->fValue >= ex(p->opr.op[1])->fValue; return pt;
        case EQ:        pt->fValue = ex(p->opr.op[0])->fValue == ex(p->opr.op[1])->fValue; return pt;
        case NE:        pt->fValue = ex(p->opr.op[0])->fValue != ex(p->opr.op[1])->fValue; return pt;

        case FOR:       for(insert(p->opr.op[0]->id.name, typeNotDefined, *ex(p->opr.op[1]), 0, 1);
                        (bool)(ex(p->opr.op[2])->fValue);
                        insert(p->opr.op[3]->id.name, typeNotDefined, *ex(p->opr.op[4]), 0 ,1)){
                            ex(p->opr.op[5]);
                            if(skipflag){
                                skipflag = 0;
                                break;
                            }
                        } return 0;
        case REPEAT:  	do{
		                    ex(p->opr.op[0]);
                        }while (ex(p->opr.op[1])->fValue); return NULL;
                    
        case SWITCH:    if(!exSwitch(p->opr.op[1], retrieve(p->opr.op[0]->id.name)->fValue, &casematch)) ex(p->opr.op[2]); skipflag = 0; return 0;

        case DEFAULT:   ex(p->opr.op[0]); return 0;
        
        case BREAK:     skipflag = 1; return 0;
        }
    }
    return 0;
}
 

bool exSwitch(nodeType *p, int switchval, bool* casematch){
    if(!p) return 0;
    if(p->opr.op[0] != NULL) exSwitch(p->opr.op[0], switchval, casematch);
    if(switchval == (int)(ex(p->opr.op[1])->fValue) && !*casematch) {
        *casematch = 1;
        ex(p->opr.op[2]);
    }
    else if(*casematch && !skipflag) ex(p->opr.op[2]);

    return *casematch;
}