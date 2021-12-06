#include "Utils.h"
#include "stdlib.h"

boolean compareArray(int* a, int* b, int len) {
  for (int i = 0; i < len; i++) {
    if (a[i] != 2 && b[i] != 2 && a[i] != b[i]) {
      return false;
    }
  }

  return true;
}
