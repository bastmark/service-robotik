#include <EEPROM.h>
#include <QTRSensors.h>
#include <Servo.h>
#include <stdlib.h>

#include "Controller.h"
#include "Gripper.h"
#include "Motor.h"
#include "Optics.h"
#include "Utils.h"

#define BASE_SPEED 80
#define BUTTON_PIN 12
#define LOOP_FREQ 10
#define TURN_TIME 540
#define TURN_LEEWAY 200

uint8_t sensorCount = 5;
uint8_t sensorArray[] = {2, 3, 4, 5, 6};
Optics optics(sensorCount, sensorArray);
Motor motor(8, 9, BASE_SPEED);
Controller controller;
Gripper gripper(10, 11);

void checkCalibration() {
  int buttonValue = digitalRead(BUTTON_PIN);

  if (buttonValue == LOW) {
    motor.drive(0, 0);
    motor.detachServos();
    digitalWrite(LED_BUILTIN, HIGH);
    optics.calibrateManual();
    digitalWrite(LED_BUILTIN, LOW);
    motor.attachServos();
  }
}

void setup() {
  // Init calibration button and led
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  delay(500);

  digitalWrite(LED_BUILTIN, HIGH);

  optics.init();
  optics.calibrateMemory();

  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(9600);

  motor.init();

  delay(1000);
}

void loop() {
  static boolean turning = false;

  float position = optics.getLinePosition();
  uint16_t* sensorValues = optics.getSensorValues();
  float control = controller.pid(position);

  float left = control > 0 ? 1.0 - control : 1;
  float right = control < 0 ? 1.0 + control : 1;

  optics.debug();

  motor.detachServos();
  gripper.grab();
  exit(0);

  // Normal loop with pid
  /*

  junction j = controller.detectJunction(sensorCount, sensorValues);

  if (j) {
          // Execute a turn
          motor.drive(1, -1);
          delay(TURN_TIME);
      motor.drive(1, 1);
          delay(TURN_LEEWAY);
  } else {
          motor.drive(left, right);
  }
  */

  checkCalibration();

  delay(LOOP_FREQ);
}
