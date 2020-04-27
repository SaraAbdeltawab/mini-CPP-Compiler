#include <unordered_map>
#include <iostream>
#include <string.h>
#include <utility>
#include <stdio.h>
#include "symboltable.h"
using namespace std;


unordered_map <string, pair<conNodeType, pair<bool,bool> >> sym;

conNodeType* insert(char* varName, conEnum lType, conNodeType value, bool constant, bool initialized, char* error) { 
   //freopen("errors.txt","a",stdout);
   
  
    string key(varName);
    cout << "insert" << key << endl;
    // Case Variable = expr;
    if (lType == typeNotDefined){
      if(sym[key].first.type == typeNotDefined){
        error = "Error: undeclared identifier name: ";
        //fclose (stdout);
        return NULL;
      }

      if (sym[key].first.type != value.type){
        error = "Error: type mismatch";
        //fclose (stdout);
        return NULL;
      }

    }else{
      // Case type Variable = expr;
      // Case const type Variable = expr;
      if (lType != value.type && value.type != typeNotDefined){
        error = "Error: type mismatch";
        //fclose (stdout);
        return NULL;
      }
    }

    // Case type Variable;
    if (value.type == typeNotDefined){
      value.type = lType;
    }

    if(sym[key].second.first != 0) constant = sym[key].second.first;
    if(sym[key].second.second != 0) initialized = sym[key].second.second;

    cout << "done insert" << key << endl;

    sym[key] = {value, {constant,initialized}};

    //fclose (stdout);
    //cout << "insert " << value.fValue << endl;
    return &sym[key].first;
  }

  conNodeType* retrieve(char* varName, char* error){

    string key(varName);
    
    //freopen("errors.txt","a",stdout);
    if(sym.find(key) == sym.end()){
        error = "Error: undeclared identifier name: ";
        //fclose (stdout);
        return NULL;
    }
    
    if(!sym[key].second.second){
        error = "Error: undeclared identifier name: ";
      //fclose (stdout);
      return NULL;
    }
   //fclose (stdout);
    return &sym[key].first;
  }
