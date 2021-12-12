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
#include "maze.h"

// base = 10kHz i think
// Actual frequency = base / LOOP_DIVIDER
#define LOOP_DIVIDER 10
// Servo value (90 max)
#define BASE_SPEED 50
#define BUTTON_PIN A0

// -------------TIMING ----------------
//560
//#define TURN_TIME 550
//#define CELL_TIME (TURN_TIME * 1.33)
//#define DELAY_BEFORE_TURN (TURN_TIME * 0.2)

unsigned long TURN_TIME = 550;
unsigned long CELL_TIME = TURN_TIME * 1.33;
unsigned long DELAY_BEFORE_TURN = TURN_TIME * 0.2;
// -------------------------------------

#define WALL_DISTANCE_END 7

int currentHeading = 3;

int initPathLength = 9;
direction initPath[] = {RIGHT, LEFT, LEFT, RIGHT, BACK, LEFT, RIGHT, RIGHT, LEFT};

uint8_t sensorCount = 5;
uint8_t sensorArray[] = {2, 4, 5, 6, 7};
Optics optics(sensorCount, sensorArray);
Motor motor(8, 3, BASE_SPEED);
Controller controller;
Gripper gripper(9, 10);
Sonics sonics(11, 12);
Maze maze(45, (uint8_t[]) {43, 42}, 2);
//5

void checkCalibrationOptics() {
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

void calibrateTurnTime() {
  float position;
  uint16_t* sensorValues;
  float control;

  junction j = NULL;

  while (!j) {
      delay(LOOP_DIVIDER);
      position = optics.getLinePosition();
      sensorValues = optics.getSensorValues();
      control = controller.pid(position);
      
      float left = control > 0 ? 1.0 - control : 1;
      float right = control < 0 ? 1.0 + control : 1;
      motor.drive(left, right);
      j = controller.detectJunction(sensorCount, sensorValues);
  }

  motor.drive(1,1);
  delay(DELAY_BEFORE_TURN);
  motor.drive(-1, 1);
  
  float pos1 = optics.getLinePosition();
  unsigned long startTime = millis();

  while (pos1 > 0.54 || pos1 < 0.46) {
    delay(LOOP_DIVIDER);
    pos1 = optics.getLinePosition();
  }
  
  unsigned long time_ = millis() - startTime - 50;

  motor.drive(0,0);
  
  TURN_TIME = time_;

  int addr = 20 * sizeof(uint16_t);
  EEPROM.put(addr, TURN_TIME);
}

void loadTurnTime() {
  EEPROM.get(20 * sizeof(uint16_t), TURN_TIME);
  CELL_TIME = TURN_TIME * 1.33;
  DELAY_BEFORE_TURN = TURN_TIME * 0.2;
}

void checkCalibrationTurn() {
  int buttonValue = digitalRead(BUTTON_PIN);

  if (buttonValue == LOW) {
    digitalWrite(LED_BUILTIN, HIGH);
    calibrateTurnTime();
    digitalWrite(LED_BUILTIN, LOW);
  }

  delay(200);
  loadTurnTime();
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
  sonics.init();
  motor.detachServos();
  delay(1000);
  motor.attachServos();

  checkCalibrationTurn();

  // Init maze
  maze.build_course_matrix();
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

  currentHeading = abs((currentHeading + mod) % 4);
}

void turn(direction d) {
  motor.drive(1, 1);
  delay(DELAY_BEFORE_TURN);

  if (d == FORWARD) {
      motor.drive(1, 1);
      return;
  }
  
  motor.drive(0, 0);
  updateHeading(d);
  
  float left = 0;
  float right = 0;

  if (d == RIGHT) {
    left = 1;
    right = -1;
  } else {
    left = -1;
    right = 1;
  }
    // Execute a turn
  int turnTime = TURN_TIME * (d == BACK ? 2 : 1);

  motor.drive(left, right);
  delay(turnTime);
  motor.drive(1, 1);
}

void end() {
  delay(50);
  exit(0);
}

// Heading from DFS to turn direction
// north: 0, east: 1, south: 2, west: 3
direction headToDir(int head) {
  int diff = (currentHeading - head) % 4;

  if (diff == 1 || diff == -3) {
    return LEFT;
  } else if (diff == 3 || diff == -1) {
    return RIGHT;
  } else if (diff == 2 || diff == -2) {
    return BACK;
  }
  
  return FORWARD;
}

direction getNextTurn(int blocksStored) {
  static int pathIdx = 0;
  
  int heading;
  direction dir;
  boolean done;

  if (pathIdx < initPathLength) {
    dir = initPath[pathIdx];
    pathIdx++;
  } else {
    done = blocksStored > 2;
    heading = done ? maze.get_turn_out() : maze.get_turn();
    dir = headToDir(heading);
  }

  return dir;
}

void testDfs() {
  int heading;
  direction dir;
  
  for (int i = 0; i < 20; i++) {
    delay(200);
    heading = maze.get_turn();
    dir = headToDir(heading);
    turn(dir);
    
    Serial.print("DFS: ");
    Serial.print(heading);
    Serial.print(" // headToDir: ");
    Serial.print(dir == FORWARD ? "FWD" : dir == LEFT ? "LFT" : dir == BACK ? "BACK" : "RGT");
    Serial.print(" // currentHeading: ");
    Serial.print(currentHeading);
    Serial.println();
    delay(100);
  }

  end();
}

void loop() {
  static int blocksStored = 0;
  unsigned long distance = sonics.getDistance();
  float position = optics.getLinePosition();
  uint16_t* sensorValues = optics.getSensorValues();
  float control = controller.pid(position);
  boolean lineVisible = optics.lineVisible();

  //testDfs();
  
  direction dir;
  boolean done;
  boolean noLineJunction;

  float left = control > 0 ? 1.0 - control : 1;
  float right = control < 0 ? 1.0 + control : 1;

  //optics.debug();

  // Normal loop with pid
  motor.drive(left, right);

  if (!lineVisible) {
    noLineJunction = distance > 15;
    
    if (noLineJunction) {
      // Lost line, drive to middle of cell
      motor.drive(1,1);
      delay(CELL_TIME);
      motor.drive(0,0);
    }
    
    dir = getNextTurn(blocksStored);
    turn(dir);

    if (noLineJunction) delay(CELL_TIME);
  }
  
  if (distance <= 4) {
    motor.drive(0,0);
    motor.detachServos();
    gripper.store();
    blocksStored += 1;
    motor.attachServos();
  }
  
  junction j = controller.detectJunction(sensorCount, sensorValues);
  
  if (j) {
    dir = getNextTurn(blocksStored);
    turn(dir);
  }

  delay(LOOP_DIVIDER);
}
