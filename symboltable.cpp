#include <unordered_map>
#include <iostream>
#include"symboltable.h"

using namespace std;




 
unordered_map <char, conNodeType> symb;
  
 void insert(char key, conNodeType value) {
    symb[key] = value;
    cout << "insert" << value.iValue;
    cout << "\ninstert" << symb[key].iValue << endl;
  }

  conNodeType retrieve(char key){
    return symb[key];
  }


