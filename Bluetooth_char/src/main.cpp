#include <Arduino.h>
byte val;
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
  if (Serial.available())
  {
    val = Serial.read();
    Serial.print("You sent me: ");
    Serial.println(val);
    if (val == 48)
    {
      digitalWrite(led, LOW);
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
    }
    else if (val == 49)
    {
      digitalWrite(led, HIGH);
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
    }
    else if (val == 50)
    {
      digitalWrite(led1, HIGH);
      digitalWrite(led, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
    }
    else if (val == 51)
    {
      digitalWrite(led2, HIGH);
      digitalWrite(led, LOW);
      digitalWrite(led1, LOW);
      digitalWrite(led3, LOW);
    }
    else if (val == 52)
    {
      digitalWrite(led3, HIGH);
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led, LOW);
    }
    else
    {
      Serial.println("Invaid input given");
    }
  }
}