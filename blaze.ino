#include <EEPROM.h>
#include <QTRSensors.h>
#include <Servo.h>
#include <stdlib.h>

#include "Controller.h"
#include "Gripper.h"
#include "Motor.h"
#include "Optics.h"
#include "Utils.h"

// base = 10kHz i think
// Actual frequency = base / LOOP_FREQ
#define LOOP_FREQ 10
// Servo value (90 max)
#define BASE_SPEED 80
#define BUTTON_PIN 12
// ms turn 90 deg
#define TURN_TIME 560
// ms delay after turn
#define TURN_LEEWAY 300

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

void printLeftRight(float l, float r) {
  Serial.print("LEFT: ");
  Serial.print(l);
  Serial.print(" RIGHT: ");
  Serial.print(r);
  Serial.println();
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

void turn(direction d) {
  float left = 0;
  float right = 0;
  
  if (d == E) {
    left = 1;
    right = -1;
  } else if (d == W) {
    left = -1;
    right = 1;
  }
    // Execute a turn
  motor.drive(left, right);
  delay(TURN_TIME);
  motor.drive(left, right);
  delay(TURN_LEEWAY);
}


void loop() {
  static boolean turning = false;

  float position = optics.getLinePosition();
  uint16_t* sensorValues = optics.getSensorValues();
  float control = controller.pid(position);

  float left = control > 0 ? 1.0 - control : 1;
  float right = control < 0 ? 1.0 + control : 1;

  //optics.debug();
  printLeftRight(left, right);

  /*motor.detachServos();
  gripper.grab();
  exit(0);*/

  // Normal loop with pid
  junction j = controller.detectJunction(sensorCount, sensorValues);

  /*switch (j) {
    case T:
      turn(E);
      break;
    case L:
      turn(E);
      break;
    case R:
      turn(W);
      break;
    default:
      motor.drive(left, right);
  }*/
  motor.drive(left, right);

  checkCalibration();

  delay(LOOP_FREQ);
}
