#include "Gripper.h"

// Gripper

Gripper::Gripper(int pinC, int pinL) {
  Gripper::pinC = pinC;
  Gripper::pinL = pinL;
}

void Gripper::grab() {
  int pos1 = 90;
  int pos2 = 75;
  
  claw.attach(pinC);

  // goes from 0 degrees to 180 degrees
  for (pos1 = 50; pos1 <= 100; pos1 += 1) {
    // in steps of 1 degree
    claw.write(pos1);          // tell servo to go to position in variable 'pos'
    delay(7);                       // waits 15 ms for the servo to reach the position
  }

  delay(200);
  
  lift.attach(pinL);

  // goes from 180 degrees to 0 degrees
  for (pos2 = 100; pos2 >= 90; pos2 -= 1) {
    lift.write(pos2);          // tell servo to go to position in variable 'pos'
    delay(3);                       // waits 15 ms for the servo to reach the position
  }
  
  delay(500);
  
  lift.detach();
}

void Gripper::store() {
  int pos1 = 90;
  int pos2 = 75;
  
  claw.attach(pinC);

  // goes from 0 degrees to 180 degrees
  for (pos1 = 50; pos1 <= 97; pos1 += 1) {
    // in steps of 1 degree
    claw.write(pos1);          // tell servo to go to position in variable 'pos'
    delay(7);                       // waits 15 ms for the servo to reach the position
  }

  delay(200);
  
  lift.attach(pinL);

  // goes from 180 degrees to 0 degrees
  for (pos2 = 100; pos2 >= 0; pos2 -= 1) {
    lift.write(pos2);          // tell servo to go to position in variable 'pos'
    delay(3);                       // waits 15 ms for the servo to reach the position
  }
  
  delay(500);

  for (pos1 = 97; pos1 >= 80; pos1 -= 1) { // goes from 180 degrees to 0 degrees
    claw.write(pos1);              // tell servo to go to position in variable 'pos'
    delay(7);                       // waits 15 ms for the servo to reach the position
  }

  delay(200);

  // goes from 0 degrees to 180 degrees
  for (pos2 = 0; pos2 <= 180; pos2 += 1) {
    // in steps of 1 degree
    lift.write(pos2);            // tell servo to go to position in variable 'pos'
    delay(3);                         // waits 15 ms for the servo to reach the position
  }

  for (pos1 = 80; pos1 >= 50; pos1 -= 1) { // goes from 180 degrees to 0 degrees
    claw.write(pos1);              // tell servo to go to position in variable 'pos'
    delay(7);                       // waits 15 ms for the servo to reach the position
  }

  lift.detach();
  
  claw.detach();
}
