#include "Motor.h"

// Motion Orchestrator

Motor::Motor(int pinL, int pinR, int speed) {
  Motor::pinL = pinL;
  Motor::pinR = pinR;
  Motor::speed = speed;
}

void Motor::attachServos() {
  left.attach(pinL);
  right.attach(pinR);
}

// Reduces risk of drift between servo calls
void Motor::detachServos() {
  left.detach();
  right.detach();
}

// l r values range (-1, 1)
void Motor::drive(float l, float r) {
  left.write(90 - l * speed);
  right.write(90 + r * speed);
}

void Motor::init() {
  attachServos();
  drive(0, 0);
}
