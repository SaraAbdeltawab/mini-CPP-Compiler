#include <unordered_map>
#include <iostream>
#include"symboltable.h"
#include<utility>
using namespace std;


unordered_map <char, pair<conNodeType, pair<bool,bool> >> sym;
  
conNodeType* insert(char key, conEnum lType, conNodeType value, bool constant, bool initialized) { 

    // Case Variable = expr;
    if (lType == typeNotDefined){
      if (sym[key].first.type != value.type){
        cout << "Error: type mismatch" << endl;
      }
      if(sym[key].first.type == typeNotDefined){
        cout << "Error: undeclared identifier" << endl;
      }
    }else{
      // Case type Variable = expr;
      // Case const type Variable = expr;
      if (lType != value.type){
        cout << "Error: type mismatch" << endl;
      }
    }

    // Case type Variable;
    if (value.type == typeNotDefined){
      value.type = lType;
    }

    if(sym[key].second.first != 0) constant = sym[key].second.first;
    if(sym[key].second.second != 0) initialized = sym[key].second.second;

    sym[key] = {value, {constant,initialized}};

    cout << "insert " << value.fValue << endl;
    return &sym[key].first;
  }

  conNodeType* retrieve(char key){
    cout << "retrieve";
    if(sym[key].first.type == typeNotDefined){
      cout << "Error: undeclared identifier name: " <<  key << endl;
    }
    
    if(!sym[key].second.second){
      cout << "Error: uninitializd identifier name: " <<  key << endl;
    }

    return &sym[key].first;
  }


