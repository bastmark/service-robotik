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

// Update heading relative to current
int updateHeading(int current, direction d) {
  int mod;

  switch (d) {
    case FORWARD:
      mod = 0;
      break;
    case RIGHT:
      mod = 1;
      break;
    case BACK:
      mod = 2;
      break;
    case LEFT:
      mod = 3;
      break;
  }

  return abs((current + mod) % 4);
}

// Heading from DFS to turn direction
// north: 0, east: 1, south: 2, west: 3
direction headToDir(int current, int head) {
  int diff = (current - head) % 4;

  if (diff == 1 || diff == -3) {
    return LEFT;
  } else if (diff == 3 || diff == -1) {
    return RIGHT;
  } else if (diff == 2 || diff == -2) {
    return BACK;
  }
  
  return FORWARD;
}

void end() {
  delay(50);
  exit(0);
}
