#ifndef Utils_h
#define Utils_h

#include "Arduino.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define CONSTR(V, MI, MA) ((V > MA) ? (MA) : (V < MI) ? MI : V)

enum junction {C, T, R, L};
enum direction {N, E, W, S};

boolean compareArray(boolean* a, boolean* b, int len);

#endif
