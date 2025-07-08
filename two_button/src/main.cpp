#include <Arduino.h>
int b1 = 8;
int b2 = 9;
int led1 = 10;
int led2 = 11;
int led3 = 12;
void setup()
{
  Serial.begin(9600);
  pinMode(b1, INPUT);
  pinMode(b2, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
}
void loop()
{
  if((digitalRead(b1)==1) && (digitalRead(b2)==1))
  {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    Serial.println("Both buttons b1 & b2 are pressed");
  }
  else if((digitalRead(b1)==1) && (digitalRead(b2)==0))
  {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    Serial.println("One button b1 is pressed");
  }
  else if((digitalRead(b1)==0) && (digitalRead(b2)==1))
  {
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW);
    Serial.println("One button b2 is pressed");
  }
  else
  {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    Serial.println("No button is pressed");
  }
  delay(1000);
}