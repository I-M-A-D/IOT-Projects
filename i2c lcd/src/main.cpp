#include <Arduino.h>
#include<Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
void setup() 
{
lcd.init();
lcd.backlight();
}
void loop()
{
lcd.setCursor(2, 0);
lcd.print("Hello, world!");
delay(1000);
lcd.clear();
lcd.setCursor(1, 1);
lcd.print("I love Arduino!");
delay(1000);
lcd.clear();
}