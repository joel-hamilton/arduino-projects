#include <Servo.h>
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;
Servo servo;
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
  DateTime now = rtc.now();
    //if (now.hour() == 15 && now.minute() == 0) {
    if(true){
    open();
    delay(1000);
    close();
    delay(60000);
  }

  delay(1000);
}

void open() {
  DateTime now = rtc.now();
  for (pos = 0; pos <= 180; pos++) {
    delay(30);
    servo.write(pos);
  }
}

void close() {
  for (pos = 180; pos >= 0; pos--) {
    servo.write(pos);
    delay(10);
  }
}
