#include "Controller.h"
#include "Utils.h"

Controller::Controller() {
  factor = 1;
  Kp = 1.9;
  Ki = -0.001;//0.003;
  Kd = 16;//16
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
  boolean l[] = {true, true, true, false, false};
  boolean r[] = {false, false, true, true, true};

  for (uint8_t i = 0; i < sensorCount; i++) {
    state[i] = (sensorValues[i] > threshold);
  }

  if (compareArray(state, t, sensorCount)) {
    Serial.print("DETECTED T: ");
    for (int i = 0; i < sensorCount; i++) {
      Serial.print(state[i]);
      Serial.print(" ");
    }
    return T;
  } else if (compareArray(state, r, sensorCount)) {
    Serial.print("DETECTED R: ");
    for (int i = 0; i < sensorCount; i++) {
      Serial.print(state[i]);
      Serial.print(" ");
    }
    return R;
  } else if (compareArray(state, l, sensorCount)) {
    Serial.print("DETECTED L: ");
    for (int i = 0; i < sensorCount; i++) {
      Serial.print(state[i]);
      Serial.print(" ");
    }
    return L;
  }

  return NULL;
}

// position range (0, 1)
float Controller::pid(float position) {
  const int window = 4;
  static float mAvg[window];
  static float lastError = 0;
  static float P, I, D = 0;
  
  // error range (-.5, .5)
  float error = 0.5 - position;
  float mean = 0;

  for (int i = 0; i < window; i++) {
    mAvg[i] = i != window-1 ? mAvg[i + 1] : error;
    mean += mAvg[i] / window;
  }

  // Turn off moving average
  // mean = error;
  
  P = mean;
  I = I + mean;
  D = mean - lastError;
  lastError = mean;
  
  float control = (P*Kp + I*Ki + D*Kd) * factor;

  return CONSTR(control, -1, 1);
}
