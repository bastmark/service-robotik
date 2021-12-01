#include <QTRSensors.h>
#include <EEPROM.h>
#include <Servo.h>
#include <stdlib.h>
#include "Motor.h"
#include "Gripper.h"
#include "Optics.h"
#include "Utils.h"
#include "Controller.h"

#define BASE_SPEED 80
#define BUTTON_PIN 12
#define LOOP_FREQ 10
#define TURN_TIME 540

// Works for speed = 80
float turnTime = (float) (380 * LOOP_FREQ) / (BASE_SPEED);
//float turnTime = (float) (24 * LOOP_FREQ) / sqrt(BASE_SPEED);

uint8_t sensorCount = 5;
uint8_t sensorArray[] = {2,3,4,5,6};
Optics optics (sensorCount, sensorArray);
Motor motor (8, 9, BASE_SPEED);
Controller controller;
Gripper gripper (10, 11);


void setup()
{

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

// Turn the robot in direction
// Returns true when turn is done
boolean turn(direction dir) {
  static int count = 0;
  
  if (dir == E) {
    motor.drive(1, -1);
  } else if (dir == W) {
    motor.drive(-1, 1);
  }

  count = (count > turnTime) ? 0 : count + 1;
  
  return (boolean) count;
}

void loop()
{
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

  //Serial.println(control);

  /*Serial.print(left);
  Serial.print(" ");
  Serial.print(right);
  Serial.println();*/

  //junction j = controller.detectJunction(sensorCount, sensorValues);

  /*if (j) {
    motor.drive(1, -1);
    delay(540);
    motor.drive(1, 1);
    delay(200);
  } else {
    
  }*/

  //motor.drive(left, right);

  /*if (j && !turning) {
    turning = true;
    motor.drive(0,0);
  }
  
  if (turning) {
    if (!turn(E)) {
      turning = false;
      motor.drive(left, right);
      delay(100);
    }
  } else {
    motor.drive(left, right);
  }*/


  int buttonValue = digitalRead(BUTTON_PIN);

  if (buttonValue == LOW) {
    motor.drive(0,0);
    motor.detachServos();
    digitalWrite(LED_BUILTIN, HIGH);
    optics.calibrateManual();
    digitalWrite(LED_BUILTIN, LOW);
    motor.attachServos();
  }

  delay(LOOP_FREQ);
}
