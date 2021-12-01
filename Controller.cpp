#include "Controller.h"
#include "Utils.h"

Controller::Controller() {
  factor = 1;
  Kp = 1.9;
  Ki = 0;//-0.001;//0.003;
  Kd = 16;//16
  factor = 1;
  lastError = 0;
/*
  Kp = 0.07;
  Ki = 0.0008;
  Kd = 0.6;
  */
  
}

junction Controller::detectJunction(uint8_t sensorCount, uint16_t* sensorValues) {
  int threshold = 800;
  boolean state[sensorCount];
  
  boolean t[] = {true, true, true, true, true};
  boolean l[] = {true, true, true, true, false};
  boolean r[] = {false, true, true, true, true};

  for (uint8_t i = 0; i < sensorCount; i++) {
    state[i] = (sensorValues[i] > threshold);
  }

  if (compareArray(state, t)) {
    return T;
  } /*else if (compareArray(state, l)) {
    return R;
  } else if (compareArray(state, r)) {
    return L;
  }*/

  return NULL;
}

// position range (0, 1)
float Controller::pid(float position) {
  
  // error range (-.5, .5)
  float error = 0.5 - position;
  
  P = error;
  I = I + error;
  D = error - lastError;
  lastError = error;
  
  float control = (P*Kp + I*Ki + D*Kd) * factor;

  return CONSTR(control, -1, 1);
}
