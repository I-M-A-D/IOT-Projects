#include <Arduino.h>
int btn1=8;
int btn2=7;
int led=9;
void setup()
{
  pinMode(led,OUTPUT);
  pinMode(btn1,INPUT);
  pinMode(btn2,INPUT);
}
void loop()
{
  if (digitalRead(btn1)==1 || digitalRead(btn2)==1)
  {
    digitalWrite(led,HIGH);
    delay(100);
    digitalWrite(led,LOW);
    delay(100);
  }
  else
  {
    digitalWrite(led,LOW);
  }
}