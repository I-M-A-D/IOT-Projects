#include <Arduino.h>
int b1 = 8;
int b2 = 9;
int num = 0;
void setup()
{
  pinMode(b1,INPUT);
  pinMode(b2,INPUT);
  Serial.begin(9600);
}
void loop()
{
  if (digitalRead(b1) == HIGH)
  {
    num++;
    Serial.print("value  of num = ");
    Serial.println(num);
    delay(100);
  }
  if (digitalRead(b2) == HIGH)
  {
    int i = 1;
    while(i<=10)
    {
      Serial.print(num);
      Serial.print(" * ");
      Serial.print(i);
      Serial.print(" = ");
      Serial.println(num*i);
      i++;
      delay(100);
    }
  }
}