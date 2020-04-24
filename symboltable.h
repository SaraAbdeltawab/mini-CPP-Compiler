#include "structnodes.h"
#ifdef __cplusplus
extern "C" {
#endif

  struct conNodeType* insert(char key, struct conNodeType value, bool constant);
  struct conNodeType* retrieve(char key);

#ifdef __cplusplus
} //end extern "C"
#endif