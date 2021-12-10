#ifndef Motor_h
#define Motor_h

#include "Arduino.h"
#include "Servo.h"
#include "Utils.h"

class Motor {
  public:
    Motor(int pinL, int pinR, int speed);
    void attachServos();
    void detachServos();
    void drive(float l, float r);
    void init();
    void setSpeed(int speed);
  private:
    int pinL;
    int pinR;
    int speed;
    Servo left;
    Servo right;
};

#endif
