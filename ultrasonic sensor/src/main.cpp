#include <Arduino.h>
const int trigPin = 9;
const int echoPin = 10;
int buzzer=4;
int yellow=5;
int red=6;
int green=7;
float duration, distance;
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  Serial.begin(9600);
}
void loop() 
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(100);
  if ((distance<=1000)&&(distance>30))
  {
    digitalWrite(buzzer, LOW);
    digitalWrite(yellow, LOW);
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
  }
  else if ((distance<=30)&&(distance>20))
  {
    digitalWrite(buzzer, LOW);
    digitalWrite(yellow, HIGH);
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);
  }
  else if ((distance<=20)&&(distance>10))
  {
    digitalWrite(buzzer, LOW);
    digitalWrite(yellow, LOW);
    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
  }
  else if (distance<=10)
  {
    digitalWrite(buzzer, HIGH);
    digitalWrite(yellow, LOW);
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);
  }
  else
  {
    digitalWrite(buzzer, LOW);
    digitalWrite(yellow, LOW);
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);
  }  
}