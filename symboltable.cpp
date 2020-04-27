#include <unordered_map>
#include <iostream>
#include <string.h>
#include <utility>
#include <stdio.h>
#include "symboltable.h"
using namespace std;

//value: variablevalue, constant, initialized
unordered_map <string, pair<conNodeType, pair<bool,bool> >> sym;

conNodeType* insert(char* varName, conEnum lType, conNodeType value, bool constant, bool initialized, char** error) { 
   
    string key(varName);

    if(sym.find(key) != sym.end() && sym[key].second.first){
      *error = (char*)malloc(55*sizeof(char)); 
      strcpy(*error, "Error: constant variable can't be reassigned name: ");
      return NULL;
    }
    // Case Variable = expr;
    if (lType == typeNotDefined){
      if(sym.find(key) == sym.end() || sym[key].first.type == typeNotDefined){
        *error = (char*)malloc(55*sizeof(char)); 
        strcpy(*error, "Error: undeclared identifier name: ");
        return NULL;
      }

      if (sym[key].first.type != value.type){
        *error = (char*)malloc(55*sizeof(char)); 
        strcpy(*error, "Error: type mismatch  with identifier ");
        return NULL;
      }

    }else{
      // Case type Variable = expr;
      // Case const type Variable = expr;
      if (lType != value.type && value.type != typeNotDefined){
        *error = (char*)malloc(55*sizeof(char)); 
        strcpy(*error, "Error: type mismatch  with identifier ");
        return NULL;
      }
    }

    // Case type Variable;
    if (value.type == typeNotDefined){
      value.type = lType;
    }

    if(sym[key].second.first != 0) constant = sym[key].second.first;
    if(sym[key].second.second != 0) initialized = sym[key].second.second;
    

    sym[key] = {value, {constant,initialized}};
    
    return &sym[key].first;
  }

  conNodeType* retrieve(char* varName, char** error){

    string key(varName);


    if(sym.find(key) == sym.end()){
        *error = (char*)malloc(55*sizeof(char)); 
        strcpy(*error, "Error: undeclared identifier name: ");
        return NULL;
    }
    
    if(!sym[key].second.second){
      *error = (char*)malloc(55*sizeof(char)); 
      strcpy(*error, "Error: unintialized identifier name: ");
      return NULL;
    }

    return &sym[key].first;
  }
