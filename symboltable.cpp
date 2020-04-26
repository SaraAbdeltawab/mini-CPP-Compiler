#include <unordered_map>
#include <iostream>
#include <string.h>
#include <utility>
#include <stdio.h>
#include "symboltable.h"
using namespace std;


unordered_map <char*, pair<conNodeType, pair<bool,bool> >> sym;

conNodeType* insert(char* key, conEnum lType, conNodeType value, bool constant, bool initialized) { 
   //freopen("errors.txt","a",stdout);
    cout << "insert";
    // Case Variable = expr;
    if (lType == typeNotDefined){
      if(sym[key].first.type == typeNotDefined){
        cout << "Error: undeclared identifier name: " <<  key << endl;
        //fclose (stdout);
        return NULL;
      }

      if (sym[key].first.type != value.type){
        cout << "Error: type mismatch" << endl;
        //fclose (stdout);
        return NULL;
      }
    }else{
      // Case type Variable = expr;
      // Case const type Variable = expr;
      if (lType != value.type && value.type != typeNotDefined){
        cout << "Error: type mismatch" << endl;
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

    sym[key] = {value, {constant,initialized}};

    //fclose (stdout);
    //cout << "insert " << value.fValue << endl;
    return &sym[key].first;
  }

  conNodeType* retrieve(char* key){
    
    //cout << "retrieve\n";
    //freopen("errors.txt","a",stdout);
    if(sym.find(key) == sym.end()){
      cout << "Error: undeclared identifier name: " <<  key << endl;
      //fclose (stdout);
      return NULL;
    }
    
    if(!sym[key].second.second){
      cout << "Error: uninitializd identifier name: " <<  key << endl;
      //fclose (stdout);
      return NULL;
    }
   //fclose (stdout);
    return &sym[key].first;
  }
