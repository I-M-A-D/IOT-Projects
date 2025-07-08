#include <Arduino.h>
int mot = 9;
int buzzer = 8;
int val0 = 0;
int val1 = 0;
int val2 = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(mot, OUTPUT);
  pinMode(buzzer, OUTPUT);
}
void loop()
{
  val0 = analogRead(A0);
  val1 = analogRead(A1);
  val2 = analogRead(A2);
  if (val2 >= 400)
  {
    Serial.print("Intensity of light is : ");
    Serial.println(val2);
    delay(500);
    if (val0 > 600)
    {
      Serial.print("value of alcohol is : ");
      Serial.println(val0);
      delay(500);
      digitalWrite(buzzer, HIGH);
    }
    else
    {
      Serial.print("value of alcohol is : ");
      Serial.println(val0);
      delay(500);
      digitalWrite(buzzer, LOW);
    }
    if (val1 >= 500)
    {
      digitalWrite(mot, HIGH);
      Serial.print("value of moisture is : ");
      Serial.println(val1);
      delay(500);
    }
    else
    {
      digitalWrite(mot, LOW);
      Serial.print("value of moisture is : ");
      Serial.println(val1);
      delay(500);
    }
  }
  else
  {
    digitalWrite(buzzer, LOW);
    digitalWrite(mot, LOW);
  }
}