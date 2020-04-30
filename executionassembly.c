#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "symboltable.h"
#include "y.tab.h"

static int lbl = 1;
const int reservedlbl = 0;
char* varName;
char* error = "";
FILE* outFile;

void execute(nodeType *p);
void yyerror(char *);

struct conNodeType* ex(nodeType *p, int oper) {
    int lbl1, lbl2;
    struct conNodeType* pt = malloc(sizeof(struct conNodeType*));
    pt->type = typeNotDefined;
    struct conNodeType* pt2;
    conEnum type;
    
    if (!p) return NULL;
    switch(p->type) {
    case typeCon:       
        pt->type = p->con.type;
        if(oper == '=') fprintf(outFile, "MOV\t%s\t", varName);
        switch(p->con.type){
            case typeInt:       pt->fValue = p->con.fValue;
                                fprintf(outFile, "%d\t", (int)p->con.fValue);  break; 
            case typeFloat:     pt->fValue = p->con.fValue;
                                fprintf(outFile, "%f\t", p->con.fValue);  break;
            case typeString:    pt->sValue = p->con.sValue;
                                fprintf(outFile, "\"%s\"\t", p->con.sValue);  break;
            case typeChar:      pt->fValue = p->con.fValue;
                                fprintf(outFile, "'%c'\t", (char)p->con.fValue);  break;
            case typeBool:      pt->fValue = p->con.fValue;
                                fprintf(outFile, "%d\t", (int)p->con.fValue);  break;
        }
        return pt;


    case typeId:      if(oper == '=') fprintf(outFile, "MOV\t%s\t", varName);
                      fprintf(outFile, "%s\t", p->id.name);
                      pt2 = retrieve(p->id.name, &error);
                      if(pt2) *pt = *pt2;
                      if (error != ""){
                          yyerror(error); 
                          error = "";
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
                        

        case '_':       if(pt){
                            pt2 = insert(p->opr.op[1]->id.name, ex(p->opr.op[0],0)->type, *pt, 0, 0, &error);
                            if(pt2) *pt = *pt2;
                        } 
                        if (error != ""){
                          yyerror(error);      
                          error = "";             
                      }
                      return pt;

        case '=':       switch (p->opr.nops)
                        {
                            case 2: 
                                    varName =  p->opr.op[0]->id.name;
                                    pt = ex(p->opr.op[1], '=');
                                    fprintf(outFile, "\n");
                                    
                                    if(pt){
                                        pt2 = insert(varName, typeNotDefined, *pt, 0, 1, &error);
                                        if(pt2) *pt = *pt2;
                                    }
                                    if (error != ""){
                                        yyerror(error);        
                                        error = "";                            
                                    }
                                    return pt;
                            
                            case 3: 
                                    pt = ex(p->opr.op[0],0);
                                    type =  pt->type;
                                    varName =  p->opr.op[1]->id.name; //var
                                    pt = ex(p->opr.op[2], '=');
                                    fprintf(outFile, "\n");
                                    if(!pt) return NULL;
                                    if(pt){
                                        pt2 = insert(varName, type, *pt, 0, 1, &error);
                                        if(pt2) *pt = *pt2;
                                    }
                                    if (error != ""){
                                        yyerror(error);   
                                        error = "";                               
                                    }
                                    return pt;
                                                
                            case 4: 
                                    pt = ex(p->opr.op[1],0);
                                    type =  pt->type;
                                    varName = p->opr.op[2]->id.name;
                                    pt = ex(p->opr.op[3], '=');
                                    fprintf(outFile, "\n");
                                    if(pt){
                                        pt = insert(p->opr.op[2]->id.name, type, *pt, 1, 1, &error);
                                        if(pt2) *pt = *pt2;
                                    } 
                                    if (error != ""){                                     
                                        yyerror(error);                                     
                                        error = "";
                                    }
                                    return pt;
                        }

        case UMINUS:    fprintf(outFile, "NFG\t");
                        return ex(p->opr.op[0],0);

        case '+':       fprintf(outFile, "ADD\t");
                        pt = ex(p->opr.op[0],0);
                        pt2 = ex(p->opr.op[1],0);
                        fprintf(outFile, "%s\n", varName);
                        if(!pt || !pt2) return pt;
                        if( pt2 && pt && (pt->type != pt2->type)) {yyerror("Error: type mismatch of the two ADD operands"); return pt;}
                        if ((pt->type == typeString || pt->type == typeChar)
                            ||
                            (pt2->type == typeString || pt2->type == typeChar)
                        ){
                            yyerror("Error: can't perform this operation on strings or char");
                            return NULL;
                        }else{
                            pt->fValue = pt->fValue + pt2->fValue;
                            return pt;
                        }
                        return pt;

        case '-':       
                        fprintf(outFile, "SUB\t");
                        pt = ex(p->opr.op[0],0);
                        pt2 = ex(p->opr.op[1],0);
                        if(!pt || !pt2) return pt;
                        if(pt && pt2 && (pt->type != pt2->type)){yyerror("Error: type mismatch of the two SUB operands"); return pt;}
                        fprintf(outFile, "%s\n", varName);
                        if ((pt->type == typeString || pt->type == typeChar)
                        ||
                            (pt2->type == typeString || pt2->type == typeChar)
                        ){
                            yyerror("Error: can't perform this operation on strings or char");
                            return NULL;
                        }else{
                            pt->fValue = pt->fValue - pt2->fValue;
                            return pt;
                        }
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
                        if(pt && pt2 && pt->type != pt2->type) yyerror("Error: type mismatch of the two AND operands");
                        fprintf(outFile, "%s\n", varName);
                        return pt;

        case OR:        //pt->fValue = ex(p->opr.op[0],0->fValue || ex(p->opr.op[1],0->fValue; return pt;
                        fprintf(outFile, "OR\t");
                        pt = ex(p->opr.op[0],0);
                        pt2 = ex(p->opr.op[1],0);
                        if(pt && pt2 && pt->type != pt2->type) yyerror("Error: type mismatch of the two OR operands");
                        fprintf(outFile, "%s\n", varName);
                        return pt;
        case '!':       //pt->fValue = !ex(p->opr.op[0],0->fValue; return pt;
                        fprintf(outFile, "NOT\t");
                        pt = ex(p->opr.op[0], 0);
                        fprintf(outFile, "%s\n", varName);
                        return pt;
                        
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
                        if(pt) insert(p->opr.op[0]->id.name, typeNotDefined, *pt, 0, 1, &error);
                        fprintf(outFile, "\nL%03d:\n", lbl1 = lbl++);
                        ex(p->opr.op[2],0);
                        fprintf(outFile, "\nJZ\tL%03d\n", lbl2 = lbl++);
                        ex(p->opr.op[5],0);
                        varName = p->opr.op[3]->id.name;
                        pt2 = ex(p->opr.op[4], 0);
                        fprintf(outFile, "\nJMP\tL%03d\n", lbl1);
                        fprintf(outFile, "L%03d:\n", lbl2);
                        if(error != "") { 
                            yyerror(error); 
                            error = "";
                        }
                        error = "";
                        if(pt2) insert(varName, typeNotDefined, *pt2, 0 ,1, &error);

                        if(error != "") {yyerror(error); error = "";}
                        return NULL;

        case REPEAT:  	fprintf(outFile, "L%03d:\n", lbl1 = lbl++);
                        ex(p->opr.op[0],0);
                        ex(p->opr.op[1],0);
                        fprintf(outFile, "\nJZ\tL%03d\n", lbl2 = lbl++);
                        fprintf(outFile, "JMP\tL%03d\n", lbl1);
                        fprintf(outFile, "L%03d:\n", lbl2);
                        return NULL;
                    
        case SWITCH:    
                        varName = p->opr.op[0]->id.name;
                        retrieve(varName, &error);
                        if(error != "") {yyerror(error); error = "";}
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

        case SYMBOLTABLE: printSymbolTable();
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
