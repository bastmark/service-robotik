#include "Utils.h"
#include "stdlib.h"

boolean compareArray(boolean* a, boolean* b) {
  int len = sizeof(a)/sizeof(boolean);
  
  for (int i = 0; i < len; i++) {
    if (a[i] != b[i]) {
      return false;
    }
  }

  return true;
}
