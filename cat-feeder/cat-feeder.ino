#include <Servo.h>
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;
Servo servo;
int button = 7;
int pos = 0;

void setup() {
  Serial.begin(9600);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
  }

  servo.attach(9); // pin 9
  servo.write(0);
}

void loop() {
  // tip on button press
  if (digitalRead(button) == HIGH) {
    feed();
  }

  DateTime now = rtc.now();
  if (now.hour() == 14 && now.minute() == 30) {
    feed();
    delay(60000);
  }
}

void feed() {
  open();
  delay(1000);
  close();
}

void open() {
  DateTime now = rtc.now();
  for (pos = 0; pos <= 180; pos++) {
    delay(20);
    servo.write(pos);
  }
}

void close() {
  for (pos = 180; pos >= 0; pos--) {
    servo.write(pos);
    delay(5);
  }
}
