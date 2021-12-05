#include "Optics.h"
#include "stdlib.h"
#include "QTRSensors.h"
#include "EEPROM.h"

QTRSensors qtr;

Optics::Optics(uint8_t sensorCount, uint8_t* sensorInputs) {
  Optics::sensorCount = sensorCount;
  Optics::sensorInputs = sensorInputs;
  Optics::sensorValues = new uint16_t[sensorCount];
  Optics::valueRange = (sensorCount - 1) * 1000;
}

void Optics::init() {
  // configure the sensors
  qtr.setTypeRC();
  qtr.setSensorPins(sensorInputs, sensorCount);
  //qtr.setEmitterPin(2);
}

void Optics::debug() {
  static boolean init = true;

  if (init) {
    Serial.print("Min/Max: ");
    for (int i = 0; i < sensorCount; i++) {
      Serial.print(qtr.calibrationOn.minimum[i]);
      Serial.print("/");
      Serial.print(qtr.calibrationOn.maximum[i]);
      Serial.print(" ");
    }
  
    Serial.println();
    init = false;
  }
  
  Serial.print("sensorValues: ");
  for (int i = 0; i < sensorCount; i++) {
    Serial.print(sensorValues[i]);
    Serial.print(" ");
  }
  Serial.println();
}

float Optics::getLinePosition() {
  return (float) qtr.readLineBlack(sensorValues) / valueRange;
}

uint16_t* Optics::getSensorValues() {
  return sensorValues;
}

void Optics::calibrateMemory() {
  qtr.calibrate();
  
  for (uint8_t i = 0; i < sensorCount; i++) {
    int addrmin = i * sizeof(uint16_t);
    int addrmax = addrmin + sensorCount * sizeof(uint16_t);
    
    EEPROM.get(addrmin, qtr.calibrationOn.minimum[i]);
    EEPROM.get(addrmax, qtr.calibrationOn.maximum[i]);
  }
}

void Optics::calibrateManual() {
  for (uint16_t i = 0; i < 400; i++) {
    qtr.calibrate();
  }

  // EEPROM.Clear();

  for (uint8_t i = 0; i < sensorCount; i++) {
    int addrmin = i * sizeof(uint16_t);
    int addrmax = addrmin + sensorCount * sizeof(uint16_t);
    
    EEPROM.put(addrmin, qtr.calibrationOn.minimum[i]);
    EEPROM.put(addrmax, qtr.calibrationOn.maximum[i]);
  }
}
