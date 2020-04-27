#include "structnodes.h"
#ifdef __cplusplus
extern "C" {
#endif

  struct conNodeType* insert(char* key,conEnum lType, struct conNodeType value, bool constant,bool initialized, char* error);
  struct conNodeType* retrieve(char* key, char* error);

#ifdef __cplusplus
} //end extern "C"
#endif