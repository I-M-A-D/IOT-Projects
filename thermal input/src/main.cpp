#include <Arduino.h>
int val = 0;
int buzzer = 8;
void setup()
{
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
}
void loop()
{
  val = analogRead(A0);
  Serial.println(val);
  delay(100);
  if (val>=900)
  {
    digitalWrite(buzzer,HIGH); 
  }
  else
  {
    digitalWrite(buzzer, LOW);
  }
}