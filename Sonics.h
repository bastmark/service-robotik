#ifndef Sonics_h
#define Sonics_h

#include "Arduino.h"

/* time between readings in ms
   On Arduino Mega readings belown 20ms interval could 
   have residual echo issues. Default 30*/
#define INTERVAL    50 

/* Scale factor round trip micro seconds per cm */
#define SCALE_CM    58
#define SCALE_CM_ROUND  (SCALE_CM/2)

/* Timeout for distance sensing rather than 1 second in us */
#define MAX_ECHO    300000
#define MIN_ECHO    (3 * SCALE_CM)

/* Timeout for start of ECHO pulse being received in us */
#define MAX_START_ECHO  1000

/* Limits for application ranges in cm */
#define MIN_RANGE   4
#define MAX_RANGE   100  //default 500

#define MAX_ERROR 10

class Sonics {
  public:
    Sonics(int pinEcho, int pinTrig);
    void init();
    unsigned long getDistance();
    unsigned long getEchoTime();
  private:
    int pinEcho;
    int pinTrig;
    unsigned long distance;
    unsigned long nextTime = INTERVAL;
};

#endif
