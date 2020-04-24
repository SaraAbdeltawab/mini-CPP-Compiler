#include <unordered_map>
#include <iostream>
#include"symboltable.h"
#include<utility>
using namespace std;


unordered_map <char, pair<conNodeType, bool>> sym;
  
conNodeType* insert(char key, conNodeType value,bool constant) { 

    if(sym[key].first.type != typeNotDefined){
       value.type = sym[key].first.type;
    }

    if(sym[key].second != 0) constant = sym[key].second;

    if(value.type == typeNotDefined){
      cout << "Error: undeclared identifier name: " <<  key << endl;
      exit(1);
    }

    sym[key] = {value, constant};

    cout << "insert " << value.fValue << endl;
    return &sym[key].first;
  }

  conNodeType* retrieve(char key){
    cout << "retrieve";
    if(value.type == typeNotDefined){
      cout << "Error: undeclared identifier name: " <<  key << endl;
      exit(1);
    }
    return &sym[key].first;
  }


