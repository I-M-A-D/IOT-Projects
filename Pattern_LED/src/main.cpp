#include<Arduino.h>
int led1=8;
int led2=9;
int led3=10;
int led4=11;
int led5=12;
int led6=13;
void setup()
{
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(led4,OUTPUT);
  pinMode(led5,OUTPUT);
  pinMode(led6,OUTPUT);
  Serial.begin(9600);
}
void loop()
{
  digitalWrite(led1,HIGH);
  digitalWrite(led2,HIGH);
  digitalWrite(led3,HIGH);
  digitalWrite(led4,HIGH);
  digitalWrite(led5,HIGH);
  digitalWrite(led6,HIGH);
  Serial.println("This is test");
  delay(1000);
  digitalWrite(led1,LOW);
  digitalWrite(led2,LOW);
  digitalWrite(led3,LOW);
  digitalWrite(led4,LOW);
  digitalWrite(led5,LOW);
  digitalWrite(led6,LOW);
  delay(1000);
  // digitalWrite(led1,HIGH);
  // delay(1000);
  // digitalWrite(led2,HIGH);
  // delay(1000);
  // digitalWrite(led3,HIGH);
  // delay(1000);
  // digitalWrite(led4,HIGH);
  // delay(1000);
  // digitalWrite(led5,HIGH);
  // delay(1000);
  // digitalWrite(led6,HIGH);
  // delay(1000);
}