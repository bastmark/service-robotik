#ifndef Gripper_h
#define Gripper_h

#include "Arduino.h"
#include "Servo.h"
#include "Utils.h"

class Gripper {
  public:
    Gripper(int pinC, int pinL);
    void grab();
    void store();
  private:
    int pinC;
    int pinL;
    Servo claw;
    Servo lift;
};

#endif
