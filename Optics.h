#ifndef Optics_h
#define Optics_h

#include "Arduino.h"

class Optics {
  public:
    Optics(uint8_t sensorCount, uint8_t sensorInputs[]);
    void init();
    void debug();
    void calibrateMemory();
    void calibrateManual();
    float getLinePosition();
    uint16_t* getSensorValues();
  private:
    uint8_t sensorCount;
    uint8_t* sensorInputs;
    uint16_t* sensorValues;
    int valueRange;
};

#endif