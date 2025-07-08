#include <Arduino.h>
int b = 8;
int led = 10;
bool state = 0;
void setup()
{
  pinMode(b, INPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}
void loop()
{
  if (digitalRead(b) == 1)
  {
    Serial.println("Button is Pressed");
    state = !state;
    digitalWrite(led, state);
    Serial.print("value of state = ");
    Serial.println(state);
    if ((state)==1)
    {
      Serial.println("LED ON");
    }
    else
    {
      Serial.println("LED OFF");
    }
    delay(1000);
  }
}