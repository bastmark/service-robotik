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
  for (pos1 = 30; pos1 <= 100; pos1 += 1) {
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
  claw.attach(pinC);
  claw.write(160);
  delay(800);

  lift.attach(pinL);
  lift.write(0);
  delay(800);

  claw.write(120);
  delay(500);

  lift.write(180);
  claw.write(110);
  delay(500);

  lift.detach();
  
  claw.detach();

  

//  // initial grab
//  for (pos1 = 100; pos1 <= 160; pos1 += 1) {
//    // in steps of 1 degree
//    claw.write(pos1);          // tell servo to go to position in variable 'pos'
//    delay(7);                       // waits 15 ms for the servo to reach the position
//  }
//
//  delay(200);
//  
//  lift.attach(pinL);
//
//  // lift the block
//  for (pos2 = 100; pos2 >= 0; pos2 -= 1) {
//    lift.write(pos2);          // tell servo to go to position in variable 'pos'
//    delay(3);                       // waits 15 ms for the servo to reach the position
//  }
//  
//  delay(500);
//
//  // release the block
//  for (pos1 = 160; pos1 >= 120; pos1 -= 1) { // goes from 180 degrees to 0 degrees
//    claw.write(pos1);              // tell servo to go to position in variable 'pos'
//    delay(7);                       // waits 15 ms for the servo to reach the position
//  }
//
//  delay(200);
//
//  // lift to init pos
//  for (pos2 = 0; pos2 <= 180; pos2 += 1) {
//    // in steps of 1 degree
//    lift.write(pos2);            // tell servo to go to position in variable 'pos'
//    delay(3);                         // waits 15 ms for the servo to reach the position
//  }
//
//  // claw to init pos
//  for (pos1 = 120; pos1 >= 100; pos1 -= 1) { // goes from 180 degrees to 0 degrees
//    claw.write(pos1);              // tell servo to go to position in variable 'pos'
//    delay(7);                       // waits 15 ms for the servo to reach the position
//  }
//
//  lift.detach();
//  
//  claw.detach();
}
