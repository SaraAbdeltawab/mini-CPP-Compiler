#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "symboltable.h"
#include "y.tab.h"


//bool exSwitch(nodeType *p, int switchval, bool* casematch);


static int lbl = 1;
const int reservedlbl = 0;
int lbl1, lbl2;
char* varName;
FILE* outFile;

void execute(nodeType *p);

struct conNodeType* ex(nodeType *p, int oper) {
    struct conNodeType* pt = malloc(sizeof(struct conNodeType*));
    pt->type = typeNotDefined;
    struct conNodeType* pt2;
    conEnum type;
    char* error = "";
    if (!p) return NULL;
    switch(p->type) {
    case typeCon:       
        pt->type = p->con.type;
        if(oper == '=') fprintf(outFile, "MOV\t%s\t", varName);
        switch(p->con.type){
            case typeInt:        
            fprintf(outFile, "%d\t", (int)p->con.fValue);  break; 
            case typeFloat:      fprintf(outFile, "%f\t", p->con.fValue);  break;
            case typeString:     fprintf(outFile, "\"%s\"\t", p->con.sValue);  break;
            case typeChar:       fprintf(outFile, "'%c'\t", (char)p->con.fValue);  break;
            case typeBool:       fprintf(outFile, "%d\t", (int)p->con.fValue);  break;
        }
        return pt;


    case typeId:      if(oper == '=') fprintf(outFile, "MOV\t%s\t", varName);
                      fprintf(outFile, "%s\t", p->id.name); 
                      pt = retrieve(p->id.name,error);
                      if (error != ""){
                          yyerror(error);
                      }
                      return pt;
                
    case typeDef:       pt->type = p->typ.type; return pt;
    case typeOpr:
        switch(p->opr.oper) {
        case WHILE: fprintf(outFile, "L%03d:\n", lbl1 = lbl++);
                    ex(p->opr.op[0],0);
                    fprintf(outFile, "\nJZ\tL%03d\n", lbl2 = lbl++);
                    ex(p->opr.op[1],0);
                    fprintf(outFile, "\nJMP\tL%03d\n", lbl1);
                    fprintf(outFile, "L%03d:\n", lbl2);
                    return NULL;

        case IF:        ex(p->opr.op[0],0);
                        fprintf(outFile, "\n");
                        if (p->opr.nops > 2) {
                            /* if else */
                            fprintf(outFile, "JZ\tL%03d\n", lbl1 = lbl++);
                            ex(p->opr.op[1],0);
                            fprintf(outFile, "JMP\tL%03d\n", lbl2 = lbl++);
                            fprintf(outFile, "L%03d:\n", lbl1);
                            ex(p->opr.op[2],0);
                            fprintf(outFile, "L%03d:\n", lbl2);
                        } else {
                            /* if */
                            fprintf(outFile, "JZ\tL%03d\n", lbl1 = lbl++);
                            ex(p->opr.op[1],0);
                            fprintf(outFile, "L%03d:\n", lbl1);
                        }
                        return NULL;

        case PRINT:     fprintf(outFile, "PRINT\t");
                        ex(p->opr.op[0],0);
                        fprintf(outFile, "\n");
                        return NULL;
        
        case ';':       ex(p->opr.op[0],0); return ex(p->opr.op[1],0);
                        

        case '_':       pt = insert(p->opr.op[1]->id.name, ex(p->opr.op[0],0)->type, *pt, 0, 0, error);
                        if (error != ""){
                          yyerror(error);
                      }
                      return pt;

        case '=':       switch (p->opr.nops)
                        {
                            case 2: //return insert(p->opr.op[0]->id.name, typeNotDefined,*ex(p->opr.op[1]), 0, 1);
                                    varName =  p->opr.op[0]->id.name;
                                    pt = ex(p->opr.op[1], '=');
                                    fprintf(outFile, "\n");
                                    pt = insert(varName, typeNotDefined, *pt, 0, 1,error);
                                    if (error != ""){
                                        yyerror(error);
                                    }
                                    return pt;
                            
                            case 3: 
                                    pt = ex(p->opr.op[0],0);
                                    type =  pt->type;
                                    // return insert(p->opr.op[1]->id.name, type, *pt, 0, 1);
                                    varName =  p->opr.op[1]->id.name;
                                    pt = ex(p->opr.op[2], '=');
                                    fprintf(outFile, "\n");
                                    if(!pt) return NULL;
                                    pt = insert(varName, type, *pt, 0, 1,error);
                                    if (error != ""){
                                        yyerror(error);
                                    }
                                    return pt;
                                                
                            case 4: 
                                    pt = ex(p->opr.op[1],0);
                                    type =  pt->type;
                                    varName = p->opr.op[2]->id.name;
                                    pt = ex(p->opr.op[3], '=');
                                    fprintf(outFile, "\n");
                                    // return insert(p->opr.op[2]->id.name, type, *pt, 1, 1);
                                    pt = insert(p->opr.op[2]->id.name, type, *pt, 1, 1,error);
                                    if (error != ""){
                                        yyerror(error);
                                    }
                                    return pt;
                        }

        case UMINUS:    
                        // pt = ex(p->opr.op[0],0;
                        // if (pt->type == typeString || pt->type == typeChar ){
                        //     fprintf(outFile, "Error: can't perform this operation on strings or char");
                        //     return NULL;
                        // }else{
                        //     pt->fValue = -(pt->fValue);
                        //     return pt;
                        // }

                        
                        fprintf(outFile, "NFG\t");
                        return ex(p->opr.op[0],0);;

        case '+':       
                        // pt = ex(p->opr.op[0],0; pt2 = ex(p->opr.op[0],0;
                        // if ((pt->type == typeString || pt->type == typeChar)
                        //     ||
                        //     (pt2->type == typeString || pt2->type == typeChar)
                        // ){
                        //     fprintf(outFile, "Error: can't perform this operation on strings or char");
                        //     return NULL;
                        // }else{
                        //     pt->fValue = pt->fValue + pt2->fValue;
                        //     return pt;
                        // }
                        fprintf(outFile, "ADD\t");
                        pt = ex(p->opr.op[0],0);
                        pt2 = ex(p->opr.op[1],0);
                        fprintf(outFile, "%s\n", varName);
                        if( pt2 && pt && (pt->type != pt2->type)) yyerror("Error: type mismatch of the two ADD operands");
                        return pt;

        case '-':       
                        fprintf(outFile, "SUB\t");
                        pt = ex(p->opr.op[0],0);
                        pt2 = ex(p->opr.op[1],0);
                        if(pt && pt2 && (pt->type != pt2->type)) yyerror("Error: type mismatch of the two SUB operands");
                        fprintf(outFile, "%s\n", varName);
                        return pt;

        case '*':       fprintf(outFile, "MUL\t");
                        pt = ex(p->opr.op[0],0);
                        pt2 = ex(p->opr.op[1],0);
                        if(pt && pt2 && (pt->type != pt2->type)) yyerror("Error: type mismatch of the two MUL operands");
                        fprintf(outFile, "%s\n", varName);
                        return pt;

        case '/':       fprintf(outFile, "DIV\t");
                        pt = ex(p->opr.op[0],0);
                        pt2 = ex(p->opr.op[1],0);
                        if(pt && pt2 && (pt->type != pt2->type)) yyerror("Error: type mismatch of the two DIV operands");
                        fprintf(outFile, "%s\n", varName);
                        return pt;


        case '%':       fprintf(outFile, "MOD\t");
                        pt = ex(p->opr.op[0],0);
                        pt2 = ex(p->opr.op[1],0);
                        if(pt && pt2 && (pt->type != pt2->type)) yyerror("Error: type mismatch of the two MOD operands");
                        fprintf(outFile, "%s\n", varName);
                        return pt;
                        
        case AND:       //pt->fValue = ex(p->opr.op[0],0->fValue && ex(p->opr.op[1],0->fValue; return pt;
                        fprintf(outFile, "AND\t");
                        pt = ex(p->opr.op[0],0);
                        pt2 = ex(p->opr.op[1],0);
                        if(ex(p->opr.op[0],0)->type != ex(p->opr.op[1],0)->type) yyerror("Error: type mismatch of the two AND operands");
                        return NULL;

        case OR:        //pt->fValue = ex(p->opr.op[0],0->fValue || ex(p->opr.op[1],0->fValue; return pt;
                        fprintf(outFile, "OR\t");
                        pt = ex(p->opr.op[0],0);
                        pt2 = ex(p->opr.op[1],0);
                        if(ex(p->opr.op[0],0)->type != ex(p->opr.op[1],0)->type) yyerror("Error: type mismatch of the two OR operands");
                        return NULL;
        case '!':       //pt->fValue = !ex(p->opr.op[0],0->fValue; return pt;
                        fprintf(outFile, "NOT\t");
                        return ex(p->opr.op[0], 0);;
                        
        case '<':     //pt->fValue = ex(p->opr.op[0],0->fValue < ex(p->opr.op[1],0->fValue; return pt;
                        fprintf(outFile, "\nCMPL\t");
                        ex(p->opr.op[0],0);
                        ex(p->opr.op[1],0);
                        return NULL;

        case '>':     //pt->fValue = ex(p->opr.op[0],0->fValue > ex(p->opr.op[1],0->fValue; return pt;
                        fprintf(outFile, "CMPG\t");
                        ex(p->opr.op[0],0);
                        ex(p->opr.op[1],0);
                        return NULL;

        case LE:      //pt->fValue = ex(p->opr.op[0],0->fValue <= ex(p->opr.op[1],0->fValue; return pt;
                        fprintf(outFile, "CMPLE\t");
                        ex(p->opr.op[0],0);
                        ex(p->opr.op[1],0);
                        return NULL;

        case GE:      //pt->fValue = ex(p->opr.op[0],0->fValue >= ex(p->opr.op[1],0->fValue; return pt;
                        fprintf(outFile, "CMPGE\t");
                        ex(p->opr.op[0],0);
                        ex(p->opr.op[1],0);
                        return NULL;

        case EQ:      //pt->fValue = ex(p->opr.op[0],0->fValue == ex(p->opr.op[1],0->fValue; return pt;
                        fprintf(outFile, "CMPE\t");
                        ex(p->opr.op[0],0);
                        ex(p->opr.op[1],0);
                        return NULL;

        case NE:      //pt->fValue = ex(p->opr.op[0],0->fValue != ex(p->opr.op[1],0->fValue; return pt;
                        fprintf(outFile, "CMPNE\t");
                        ex(p->opr.op[0],0);
                        ex(p->opr.op[1],0);
                        return NULL;

        case FOR:       fprintf(outFile, "MOV\t%s\t", p->opr.op[0]->id.name);
                        pt = ex(p->opr.op[1],0);
                        fprintf(outFile, "\nL%03d:\n", lbl1 = lbl++);
                        ex(p->opr.op[2],0);
                        fprintf(outFile, "\nJZ\tL%03d\n", lbl2 = lbl++);
                        ex(p->opr.op[5],0);
                        varName = p->opr.op[3]->id.name;
                        pt2 = ex(p->opr.op[4], 0);
                        fprintf(outFile, "\nJMP\tL%03d\n", lbl1);
                        fprintf(outFile, "L%03d:\n", lbl2);

                        insert(p->opr.op[0]->id.name, typeNotDefined, *pt, 0, 1,error);
                        if(error != "") yyerror(error);
                        error = "";
                        insert(varName, typeNotDefined, *pt2, 0 ,1,error);
                        if(error != "") yyerror(error);
                        return NULL;

        case REPEAT:  	fprintf(outFile, "L%03d:\n", lbl1 = lbl++);
                        ex(p->opr.op[0],0);
                        ex(p->opr.op[1],0);
                        fprintf(outFile, "JZ\tL%03d\n", lbl2 = lbl++);
                        fprintf(outFile, "JMP\tL%03d\n", lbl1);
                        fprintf(outFile, "L%03d:\n", lbl2);
                        return NULL;
                    
        case SWITCH:    //if(!exSwitch(p->opr,''.op[1], retrieve(p->opr.op[0]->id.name)->fValue, &casematch)) ex(p->opr.op[2],0; skipflag = 0; return 0;
                        varName = p->opr.op[0]->id.name;
                        retrieve(varName,error);
                        if(error != "") yyerror(error);
                        fprintf(outFile, "MOV R0, 0\n");
                        ex(p->opr.op[1], 0);
                        ex(p->opr.op[2], 0);
                        fprintf(outFile, "\nL%03d:\n", reservedlbl);
                        return NULL;
        
        case CASE:      
                        ex(p->opr.op[0], CASE);
                        fprintf(outFile, "CMP R0 1\n");
                        fprintf(outFile, "JE\tL%03d\n", lbl1 = lbl++);
                        fprintf(outFile, "CMP\t%s\t", varName);
                        ex(p->opr.op[1], CASE);
                        fprintf(outFile, "\nJZ\tL%03d\n", lbl2 = lbl++);
                        fprintf(outFile, "MOV R0 1");
                        fprintf(outFile, "\nL%03d:\n", lbl1);
                        ex(p->opr.op[2], CASE);
                        fprintf(outFile, "\nL%03d:\n", lbl2);
                        return NULL;

        case DEFAULT:   fprintf(outFile, "L%03d:\n", lbl2 = lbl++);
                        ex(p->opr.op[0],0); return NULL;
        
        case BREAK:     if(CASE) fprintf(outFile, "JMP\tL%03d\n", reservedlbl);
                        else fprintf(outFile, "JMP\tL%03d\n", lbl2);
                        return NULL;
        }
    }
    return 0;
}
 

void execute(nodeType *p){
    outFile = fopen("quadruples.txt", "a");
    ex(p, 0);
    fclose(outFile);
}


// bool exSwitch(nodeTy,''pe *p, int switchval, bool* casematch){
//     if(!p) return 0;
//     if(p->opr.op[0] != NULL) exSwitch(p->opr,''.op[0], switchval, casematch);
//     if(switchval == (int)(ex(p->opr.op[1],0->fValue) && !*casematch) {
//         *casematch = 1;
//         ex(p->opr.op[2])0/     }
//     else if(*casematch && !skipflag) ex(p->opr.op[2])0//     return *casematch;
// }