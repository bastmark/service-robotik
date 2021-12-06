#include "Sonics.h"
#include "Utils.h"

Sonics::Sonics(int pinEcho, int pinTrig) {
  Sonics::pinEcho = pinEcho;
  Sonics::pinTrig = pinTrig;
}

void Sonics::init() {
  pinMode(pinTrig, OUTPUT);
  digitalWrite(pinTrig, LOW);
  pinMode(pinEcho, INPUT);

  Serial.println(F("PC Services - Better Range test"));

  distance = getEchoTime();
  
  if(distance > 0 && distance <= 10){
    Serial.println( F( "No unit found - Error = " ) );
    Serial.println( distance );
    delay(500);
    exit(0);
  }
}

unsigned long Sonics::getEchoTime() {
  unsigned long start_time;
  unsigned long end_time;
  
  /* check Echo if high return error */
  if(digitalRead(pinEcho)) return(2);
    
  /* note start time */
  start_time = micros( );
  
  /* send the trigger pulse */
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);
  
  /* Set timeout for start of echo pulse */
  end_time = start_time + MAX_START_ECHO;
  
  /* check ECHO pin goes high within MAX_START_ECHO
     if not return error of 1  */
  while(!digitalRead(pinEcho) )
     if(micros() > end_time) return(1);
  
  /* Check for Length of echo occurred or timeout */
  start_time = micros( );
  end_time = start_time + MAX_ECHO;
  
  while(digitalRead(pinEcho))
     if(micros() > end_time) break;
       
  end_time = micros( );
       
  /* Return time or timeout   */
  return((start_time < end_time ) ? end_time - start_time: 0);
}

unsigned long Sonics::getDistance() {
  unsigned long newTime = millis();
  
  if(newTime >= nextTime) {
    /* Calculate distance */
    unsigned int echotime = getEchoTime();
    
    /* only scale valid readings 0 is timeout or 1 is no echo
       realistically minimum accurate or physical range is 3cm */
    if(echotime > MAX_ERROR) {
      // Valid number covert to cm
      distance = echotime;
      distance += SCALE_CM_ROUND;  // add in half bit rounding  
      distance /= SCALE_CM;
    }
    
    nextTime = newTime + INTERVAL; // save next time to run
  }

  return distance;
}
