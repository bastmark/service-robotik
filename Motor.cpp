#include "Motor.h"

// Motion Orchestrator

Motor::Motor(int pinL, int pinR, int speed) {
  Motor::pinL = pinL;
  Motor::pinR = pinR;
  Motor::speed = speed;
}

void Motor::setSpeed(int speed) {
  Motor::speed -= speed;
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
  left.writeMicroseconds(1500 + l * speed);
  right.writeMicroseconds(1500 - r * speed * 0.8);
//  left.write(90 + l * speed);
//  right.write(90 - r * speed);
}

void Motor::pid(float position) {
  float factor = 1;
  float Kp = 3;//3;//1.6;//1.9;
  float Ki = -0.001;//0.003;
  float Kd = 16;//25;//30;//16
  
  const int window = 1;
  static float mAvg[window];
  static float lastMean = 0;
  static float P, I, D = 0;
  
  // error range (-.5, .5)
  float error = 0.5 - position;
  float mean = 0;

  for (int i = 0; i < window; i++) {
    mAvg[i] = i != window-1 ? mAvg[i + 1] : error;
    mean += mAvg[i] / window;
  }

  // Turn off moving average
  //mean = error;
  
  P = mean;
  I = I + mean;
  D = mean - lastMean;
  lastMean = mean;
  
  float control = CONSTR((P*Kp + I*Ki + D*Kd) * factor, -1, 1);
  
  float left = control > 0 ? 1.0 - control : 1;
  float right = control < 0 ? 1.0 + control : 1;
  
  drive(left, right);
}

void Motor::init() {
  attachServos();
  drive(0, 0);
}
