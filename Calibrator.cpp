#include "Calibrator.h"

boolean waitForButton(int buttonPin) {
  unsigned long startT, endT;
  int buttonValue;

  startT = millis();
  endT = millis();
  buttonValue = digitalRead(buttonPin);

  while ((endT - startT) < 1000 && buttonValue == HIGH) {
    buttonValue = digitalRead(buttonPin);
    endT = millis();
    delay(10);
    Serial.println("hello");
  }

  return (buttonValue == LOW);
}

void blinkLed(int n, int d) {
  for (int i = 0; i < n; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(d);
    digitalWrite(LED_BUILTIN, LOW);
    delay(d);
  }
}

int calibrationMenu(int buttonPin) {
  delay(500);

  blinkLed(2, 500);

  if (waitForButton(buttonPin)) {
    return 1;
  }

  blinkLed(3, 500);

  if (waitForButton(buttonPin)) {
    return 2;
  }
}
