#include <LiquidCrystal.h>

LiquidCrystal lcd(41, 40, 34, 33, 32, 31);
//int portpin = A0;
int bv = 0;
int iv = 0;
float bvp = 0.0;
float ivp = 0.0;


void setup() {
lcd.begin(16, 2);

lcd.setCursor(2, 0);
lcd.print("Anten Traker");
lcd.setCursor(4, 1);
lcd.print("TEKNOMER");
delay(4000);
lcd.clear();
}

void loop() {
lcd.clear();
bv = analogRead(A1); 
bvp = bv * 0.007173; 
lcd.setCursor(0, 0);
lcd.print("5 volt ");
lcd.print(bvp);  

iv = analogRead(A0); 
ivp = iv * 0.02765; 
lcd.setCursor(0, 1);
lcd.print("24 volt ");
lcd.print(ivp); 
delay(1000); 
}
