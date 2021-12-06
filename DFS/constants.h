#ifndef __constants_h__
#define __constants_h__


#ifdef TEST

#define VERBOSE_DFS
#define VERBOSE_STACK

#include <iostream>
#define print_out(x) std::cout << (x);

#include <time.h>
#define random(l, u) (u - 1 - (rand() % (u - i)))

#define HIGH 1
#define LOW 0
#define OUTPUT 1
#define INPUT 0

#define digitalWrite(x,y)
#define pinMode(x,y)

#include <unistd.h>
#define delayMicroseconds(x) usleep((x)/1000000)

#define analogRead(x) 0
#define micros() 0

#else /* TEST */

#include "Arduino.h"
#define print_out(x) Serial.print(x);

#endif /* TEST */



#define NUM_ROWS 3
#define NUM_COLS 4



#endif /* __constants_h__ */
