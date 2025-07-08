#include <Arduino.h>
int led = 13;
void setup()

{
  pinMode(led,OUTPUT);
  Serial.begin(9600);
}
void loop()
{
  digitalWrite(led,HIGH);
  Serial.println("LED is ON");
  delay(1000);
  digitalWrite(led,LOW);
  Serial.println("LED is OFF");
  delay(1000);
}