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
// Actual frequency = base / LOOP_DIVIDER
#define LOOP_DIVIDER 10
// Servo value (90 max)
#define BASE_SPEED 90
#define BUTTON_PIN A0
// ms turn 90 deg
#define TURN_TIME 600
#define CELL_TIME 800
// ms delay after turn
#define TURN_LEEWAY 200
#define WALL_DISTANCE_END 7

// Behaviour
#define JUNCTION_DETECTION true
#define KILL_ON_SIGHT true

direction path[] = {E, W, W, E, W, E, E, W};

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

void checkButton(boolean* stop) {
  int buttonValue = digitalRead(BUTTON_PIN);

  if (buttonValue == LOW) {
    Serial.println("BUTTONPRESS");
    if (stop) {
      *stop = false;
    } else {
      *stop = true;
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
  motor.detachServos();
  delay(1000);
  motor.attachServos();
}

void turn(direction d) {
  motor.drive(0, 0);
  
  float left = 0;
  float right = 0;
  
  if (d == E) {
    left = 1;
    right = -1;
  } else {
    left = -1;
    right = 1;
  }
    // Execute a turn
  motor.drive(left, right);
  int turnTime = TURN_TIME * (d == S ? 2 : 1);
  int leeWay = TURN_LEEWAY * (d == S ? 0 : 1);
  
  delay(turnTime);
  motor.drive(1, 1);
  delay(TURN_LEEWAY);
}

//void turn(direction d) {
//  motor.drive(0, 0);
//  
//  float left = 0;
//  float right = 0;
//  
//  if (d == E) {
//    left = 1;
//    right = -1;
//  } else {
//    left = -1;
//    right = 1;
//  }
//    // Execute a turn
//  motor.drive(left, right);
//  delay(TURN_LEEWAY);
//  float position = optics.getLinePosition();
//
//  while (position > 0.52 | position < 0.48) {
//    delay(LOOP_DIVIDER);
//    position = optics.getLinePosition();
//  }
//
//  motor.drive(0,0);
//}

unsigned long calibrateTurnTime() {
  motor.drive(-1,1);
  unsigned long startTime = millis();

  delay(500);
  
  float position = optics.getLinePosition();
  
  while(position != 0.5) {
    position = optics.getLinePosition();
  }

  unsigned long endTime = millis();
  motor.drive(0,0);

  return startTime - endTime;
}

void end() {
  delay(50);
  exit(0);
}


void loop() {
  static int pathIdx = 0;
  unsigned long distance = sonics.getDistance();
  float position = optics.getLinePosition();
  uint16_t* sensorValues = optics.getSensorValues();
  float control = controller.pid(position);
  boolean lineVisible = optics.lineVisible();

  float left = control > 0 ? 1.0 - control : 1;
  float right = control < 0 ? 1.0 + control : 1;

  if (!lineVisible) {
    if (distance > 8) {
      // Lost line, drive to middle of cell
      motor.drive(1,1);
      delay(CELL_TIME);
      turn(E);
      motor.drive(1,1);
      delay(CELL_TIME);
    } else {
      // Dead end, turn around
      turn(S);
      end();
    }
  }

  // Normal loop with pid
  motor.drive(left, right);
  
  if (KILL_ON_SIGHT && distance <= 4) {
    motor.drive(0,0);
    motor.detachServos();
    gripper.store();
    motor.attachServos();
    turn(S);
  }

  if (JUNCTION_DETECTION) {
    junction j = controller.detectJunction(sensorCount, sensorValues);

    if (j) {
      direction next = path[pathIdx];
      turn(next);
      pathIdx++; 
    }
  }

  delay(LOOP_DIVIDER);
}
