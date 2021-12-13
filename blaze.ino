#include <EEPROM.h>
#include <QTRSensors.h>
#include <Servo.h>
#include <stdlib.h>

#include "Gripper.h"
#include "Motor.h"
#include "Optics.h"
#include "Utils.h"
#include "Sonics.h"
#include "Calibrator.h"
#include "maze.h"

// base = 10kHz i think
// Actual frequency = base / LOOP_DIVIDER
#define LOOP_DIVIDER 10
// Servo value (90 max)
#define BASE_SPEED 300
#define BUTTON_PIN A0

// -------------TIMING ----------------
unsigned long TURN_TIME = 550;
unsigned long CELL_TIME = TURN_TIME * 1.33;
unsigned long DELAY_BEFORE_TURN = TURN_TIME * 0.2;
// -------------------------------------

int currentHeading = 3;
int blocksStored = 0;

int initPathLength = 9;
direction initPath[] = {RIGHT, LEFT, LEFT, RIGHT, BACK, LEFT, RIGHT, RIGHT, LEFT};

uint8_t sensorCount = 5;
uint8_t sensorArray[] = {2, 3, 4, 7, 8};

Optics optics(sensorCount, sensorArray);
Motor motor(6, 5, BASE_SPEED);
Gripper gripper(9, 10);
Sonics sonics(11, 12);
Maze maze(45, (uint8_t[]) {43, 42}, 2);
//5

void calibrateTurnTimeNew() {
  boolean done = false;
  int addr = 20 * sizeof(uint16_t);
  int buttonValue;
  unsigned long distance;
  
  while (!done) {
    // Drive until junction
    junction j = NULL;
  
    while (!j) {
        delay(LOOP_DIVIDER);
        motor.pid(optics.getLinePosition());
        j = optics.detectJunction();
    }
  
    // Do the turn
    motor.drive(1,1);
    delay(DELAY_BEFORE_TURN);
    motor.drive(-1, 1);
    delay(TURN_TIME);
    motor.drive(0,0);

    buttonValue = digitalRead(BUTTON_PIN);

    while (buttonValue == HIGH) {
      buttonValue = digitalRead(BUTTON_PIN);
      distance = sonics.getDistance();
      delay(10);
    }

    if (distance < 15) {
      TURN_TIME += 10;
    } else {
      TURN_TIME -= 10;
    }

    delay(100);
    EEPROM.put(addr, TURN_TIME);
    delay(3000);
  }
  
}

void loadTurnTime() {
  EEPROM.get(20 * sizeof(uint16_t), TURN_TIME);
  CELL_TIME = TURN_TIME * 1.33;
  DELAY_BEFORE_TURN = TURN_TIME * 0.2;
}

void setup() {
  // Init calibration button and led
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  delay(500);

  Serial.begin(9600);

  optics.init();
  motor.init();
  sonics.init();
  motor.detachServos();
  delay(1000);
  motor.attachServos();

  digitalWrite(LED_BUILTIN, HIGH);

  int buttonValue = digitalRead(BUTTON_PIN);
  int selection = 0;

  digitalWrite(LED_BUILTIN, LOW);

  if (buttonValue == LOW) {
    selection = calibrationMenu(BUTTON_PIN);
  }

  if (selection == 1) {
    digitalWrite(LED_BUILTIN, HIGH);
    optics.calibrateManual();
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    optics.calibrateMemory();
  }

  loadTurnTime();

  if (selection == 2) {
    calibrateTurnTimeNew();
  }

  loadTurnTime();

  // Init maze
  maze.build_course_matrix();
}

void turn(direction d) {
  motor.drive(1, 1);
  delay(DELAY_BEFORE_TURN);

  if (d == FORWARD) {
      motor.drive(1, 1);
      return;
  }
  
  motor.drive(0, 0);
  currentHeading = updateHeading(currentHeading, d);
  
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

direction getNextTurn() {
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
    dir = headToDir(currentHeading, heading);
  }

  return dir;
}

void testDfs() {
  int heading;
  direction dir;
  
  for (int i = 0; i < 20; i++) {
    delay(200);
    heading = maze.get_turn();
    dir = headToDir(currentHeading, heading);
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
  unsigned long distance = sonics.getDistance();
  float position = optics.getLinePosition();
  uint16_t* sensorValues = optics.getSensorValues();
  boolean lineVisible = optics.lineVisible();
  
  direction dir;
  boolean done;
  boolean noLineJunction;

  optics.debug();

  // Normal loop with pid
  motor.pid(position);
  //motor.drive(0,0);

  if (!lineVisible) {
    noLineJunction = distance > 15;
    
    if (noLineJunction) {
      // Lost line, drive to middle of cell
      motor.drive(1,1);
      delay(CELL_TIME);
      motor.drive(0,0);
    }
    
    dir = getNextTurn();
    turn(dir);

    if (noLineJunction) delay(CELL_TIME);
  }
  
  if (distance <= 4) {
    delay(120);
    motor.drive(0,0);
    gripper.store();
    blocksStored += 1;
  }
  
  junction j = optics.detectJunction();
  
  if (j) {
    dir = getNextTurn();
    turn(dir);
  }

  if (digitalRead(BUTTON_PIN) == LOW) {
    motor.drive(0,0);
    delay(5000);
  }

  delay(LOOP_DIVIDER);
}
