#ifndef Gripper_h
#define Gripper_h

#include "Arduino.h"
#include "Servo.h"

class Gripper {
  public:
    Gripper(int pinC, int pinL);
    void grab();
  private:
    int pinC;
    int pinL;
    Servo claw;
    Servo lift;
};

#endif
