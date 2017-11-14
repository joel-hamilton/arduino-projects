#include <LiquidCrystal.h>
#include <dht11.h>
dht11 DHT11;
#define DHT11PIN 8
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int ThermistorPin = 0;
int Vo;
float R1 = 10000;
float logR2, R2, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
}
void loop()
{
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T = T - 273.15;
  // T = (T * 9.0)/ 5.0 + 32.0; 
  lcd.setCursor(0, 0);
  //lcd.print("Temp: ");
  //lcd.print(T);
  //lcd.print("C");
  delay(100);
  DHT11.read(DHT11PIN);
  lcd.print("Temp: ");
  lcd.print(DHT11.temperature);
  lcd.setCursor(1,0);
  lcd.print("Humidity: ");
  lcd.print(DHT11.humidity):
}
