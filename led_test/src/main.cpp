#include <Arduino.h>

int led=7;
void setup()
{
  pinMode(led,OUTPUT);
}
void loop()
{
  digitalWrite(led,HIGH);
}