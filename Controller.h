#ifndef Controller_h
#define Controller_h

#include "Arduino.h"
#include "Utils.h"

class Controller {
  public:
    Controller();
    float pid(float position);
    junction detectJunction(uint8_t sensorCount, uint16_t* sensorValues);
  private:
    float Kp;
    float Ki;
    float Kd;
    float factor;
    float lastError;
    float P;
    float I;
    float D;
};

#endif
