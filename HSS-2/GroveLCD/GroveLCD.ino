#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

void setup() 
{
    lcd.begin(16, 2);
    lcd.print("deneme123");
    lcd.setCursor(2,1);
    lcd.print("123deneme");
}

void loop() 
{
   
}
