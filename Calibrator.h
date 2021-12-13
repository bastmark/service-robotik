#ifndef Calibrator_h
#define Calibrator_h

#include "Arduino.h"
#include "Utils.h"
#include "Optics.h"

boolean waitForButton(int buttonPin);
int calibrationMenu(int buttonPin);
void blinkLed(int n, int d);

#endif
