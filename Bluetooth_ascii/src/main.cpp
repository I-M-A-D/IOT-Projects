#include <Arduino.h>
char val;
int led = 8;
int led1 = 9;
int led2 = 10;
int led3 = 11;
void setup()
{
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
}
void loop()
{
  if (Serial.available() > 0)
  {
    val = Serial.read();
    Serial.print("You have pressed :");
    Serial.println(val);
    switch (val)
    {
    case '0':
      digitalWrite(led, LOW);
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      break;
    case '1':
      digitalWrite(led, HIGH);
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      break;
    case '2':
      digitalWrite(led, LOW);
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      break;
    case '3':
      digitalWrite(led, LOW);
      digitalWrite(led1, LOW);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, LOW);
      break;
    case '4':
      digitalWrite(led, LOW);
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, HIGH);
      break;
    default:
      Serial.println("Undefine input");
      break;
    }
  }
}