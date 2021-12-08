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

int currentHeading = 3;
direction path[] = {RIGHT, LEFT, LEFT, RIGHT, LEFT, RIGHT, RIGHT, LEFT};
int hPath[] = {0, 3, 2, 3, 0, 1, 2, 1};

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

void updateHeading(direction d) {
  int mod;

  switch (d) {
    case FORWARD:
      mod = 0;
      break;
    case RIGHT:
      mod = 1;
      break;
    case BACK:
      mod = 2;
      break;
    case LEFT:
      mod = 3;
      break;
  }

  currentHeading = (currentHeading + mod) % 4;
}

void turn(direction d) {
  motor.drive(0, 0);
  updateHeading(d);
  
  float left = 0;
  float right = 0;

  if (d == FORWARD) {
    return;
  } else if (d == RIGHT) {
    left = 1;
    right = -1;
  } else {
    left = -1;
    right = 1;
  }
    // Execute a turn
  motor.drive(left, right);
  int turnTime = TURN_TIME * (d == BACK ? 2 : 1);
  int leeWay = TURN_LEEWAY * (d == BACK ? 0 : 1);
  
  delay(turnTime);
  motor.drive(1, 1);
  delay(TURN_LEEWAY);
}

void end() {
  delay(50);
  exit(0);
}

// Heading from DFS to turn direction
// north: 0, east: 1, south: 2, west: 3
direction headToDir(int head) {
  int diff = (currentHeading - head) % 3;

  if (diff == 3) {
    return LEFT;
  } else if (diff == 1) {
    return RIGHT;
  } else if (diff == 2) {
    return BACK;
  }
  
  return FORWARD;
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

  Serial.println(currentHeading);

  if (!lineVisible) {
    if (distance > 8) {
      // Lost line, drive to middle of cell
      motor.drive(1,1);
      delay(CELL_TIME);
      turn(RIGHT);
      motor.drive(1,1);
      delay(CELL_TIME);
    } else {
      // Dead end, turn around
      turn(BACK);
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
    turn(BACK);
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
