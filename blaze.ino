#include <EEPROM.h>
#include <QTRSensors.h>
#include <Servo.h>
#include <stdlib.h>

#include "Controller.h"
#include "Gripper.h"
#include "Motor.h"
#include "Optics.h"
#include "Utils.h"
#include "Sonics.h"

// base = 10kHz i think
// Actual frequency = base / LOOP_FREQ
#define LOOP_FREQ 10
// Servo value (90 max)
#define BASE_SPEED 50
#define BUTTON_PIN A0
// ms turn 90 deg
#define TURN_TIME 600
// ms delay after turn
#define TURN_LEEWAY 300

// Behaviour
#define JUNCTION_DETECTION false
#define GRAB_ON_SIGHT false

boolean stop = true;

uint8_t sensorCount = 5;
uint8_t sensorArray[] = {2, 3, 4, 5, 6};
Optics optics(sensorCount, sensorArray);
Motor motor(7, 8, BASE_SPEED);
Controller controller;
Gripper gripper(9, 10);
Sonics sonics(12, 11);

void checkCalibration() {
  int buttonValue = digitalRead(BUTTON_PIN);

  if (buttonValue == LOW) {
    motor.drive(0, 0);
    motor.detachServos();
    digitalWrite(LED_BUILTIN, HIGH);
    optics.calibrateManual();
    digitalWrite(LED_BUILTIN, LOW);
    motor.attachServos();
  } else {
    optics.calibrateMemory();
  }
}

void checkButton() {
  int buttonValue = digitalRead(BUTTON_PIN);

  if (buttonValue == LOW) {
    Serial.println("BUTTONPRESS");
    if (stop) {
      stop = false;
    } else {
      stop = true;
    }
    delay(500);
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
  checkCalibration();

  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(9600);

  motor.init();
  sonics.init();

  delay(1000);
}

void turn(direction d) {
  motor.drive(0, 0);
  
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
  motor.drive(1, 1);
  delay(TURN_LEEWAY);
}


void loop() {
  unsigned long distance = sonics.getDistance();
  float position = optics.getLinePosition();
  uint16_t* sensorValues = optics.getSensorValues();
  float control = controller.pid(position);

  float left = control > 0 ? 1.0 - control : 1;
  float right = control < 0 ? 1.0 + control : 1;

  // Normal loop with pid
  if (!stop) {
    motor.drive(left, right);
    
    if (GRAB_ON_SIGHT && distance <= 4) {
      motor.drive(0,0);
      motor.detachServos();
      gripper.grab();
      motor.attachServos();
    }

    if (JUNCTION_DETECTION) {
      switch (controller.detectJunction(sensorCount, sensorValues)) {
        case T:
          turn(E);
          break;
        case L:
          //turn(W);
          turn(E);
          break;
        case R:
          turn(E);
          break;
      }
    }
  } else {
    motor.drive(0,0);
  }

  checkButton();

  delay(LOOP_FREQ);
}
