#include <LiquidCrystal.h>
LiquidCrystal lcd(41, 40, 34, 33, 32, 31);
 
String gelenseri = "";
String acilar = "";
boolean stringComplete = false;  // dizenin eksiksiz olup olmadığı



void setup() {
Serial3.begin(9600); //seri3 port

}

void loop() {
 //acilar = "";
while (Serial3.available()) {
char inChar = (char)Serial3.read(); 
gelenseri += inChar;
if (inChar == '\n') {
stringComplete = true;
} 
}
if (stringComplete) {
if (gelenseri.startsWith("")){
// Serial.print(sonarveri);
acilar = gelenseri;
}   
gelenseri = "";
stringComplete = false;
}
lcd.clear(); 
lcd.print(acilar);
delay (100);
}
