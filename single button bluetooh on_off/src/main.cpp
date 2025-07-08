#include <Arduino.h>
char val;
int led1 = 8;
int led2 = 9;
int led3 = 10;
bool state1 = 0;
bool state2 = 0;
bool state3 = 0;
void setup()
{
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  Serial.begin(9600);
}
void loop()
{
  if (Serial.available())
  {
    val = Serial.read();
    Serial.print("You have entered: ");
    Serial.println(val);

    if ((val) == '1')
    {
      state1 = !state1;
      digitalWrite(led1, state1);
      if ((state1) == 1)
      {
        Serial.println("LED1 ON");
      }
      else
      {
        Serial.println("LED1 OFF");
      }
      delay(1000);
    }
    if ((val) == '2')
    {
      state2 = !state2;
      digitalWrite(led1, state1);
      if ((state2) == 1)
      {
        Serial.println("LED2 ON");
      }
      else
      {
        Serial.println("LED2 OFF");
      }
      delay(1000);
    }
    if ((val) == '3')
    {
      state3 = !state3;
      digitalWrite(led1, state1);
      if ((state3) == 1)
      {
        Serial.println("LED3 ON");
      }
      else
      {
        Serial.println("LED3 OFF");
      }
      delay(1000);
    }
  }
}