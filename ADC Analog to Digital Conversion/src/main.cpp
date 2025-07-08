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
  if ((val>=100) && (val<300))
  {
    digitalWrite(buzzer,HIGH); 
    delay(1000);
    digitalWrite(buzzer, LOW);
    delay(1000);
    
  }

  else if ((val>=300) && (val<500))
  {
    digitalWrite(buzzer, HIGH);
    delay(700);
    digitalWrite(buzzer, LOW);
    delay(700);
  }
  else if ((val>=500) && (val<800)) 
  {
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    delay(500);
  }
  else if ((val>=800) && (val<900))
  {
    digitalWrite(buzzer, HIGH);
    delay(300);
    digitalWrite(buzzer, LOW);
    delay(300);
  }
  else if (val>=900)
  {
    digitalWrite(buzzer, HIGH);
    
  }
  else
  {
    digitalWrite(buzzer, LOW);
  }
}