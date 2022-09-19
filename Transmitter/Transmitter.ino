#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 6, 5, 4, 3); 
#define button 8
#define xPin A0
#define yPin A1
int valueX  = 0;
int valueY  = 0;

void setup(){
  lcd.begin(16, 2);
  lcd.clear();
  print("BMS", 0);
  print("ANTEN TRACKER", 1);
  delay(2000);
  lcd.clear();
  Serial.begin(9600);
  print("   Y        X   ", 0);
}

void loop(){
  delay(100);
  valueX  = analogRead(xPin) * 0.3027343; // 0 -309
  valueY  = analogRead(yPin) * 0.1220703; // 0 - 124;
  printDegree(valueX, 11, 14, true);
  printDegree(valueY, 2, 4, false);
  if (digitalRead(button)){
    Serial.print(valueX);
    Serial.print(".");
    Serial.println(valueY);
    lcd.setCursor(7, 0);
    lcd.print("+");
    delay(250);
    lcd.print(" ");
  } 
}

void printDegree(int degree, byte iBot, byte iTop, bool isDegreeX){
  for(byte i = iBot; i < iTop; i++){
    lcd.setCursor(i, 1);
    lcd.print(" ");
  }
  if(degree < 10){
    if(isDegreeX){
      lcd.setCursor(12, 1);
    }else{
      lcd.setCursor(3, 1);
    } 
  }else{
    if(isDegreeX){
      lcd.setCursor(11, 1);
    }else{
      lcd.setCursor(2, 1);
    }
  }
  lcd.print(degree);
}

void print(String text, byte line){
  for(byte i = 0; i < 16; i++){
    lcd.setCursor(i, (line % 2));
    lcd.print(" ");
  }
  lcd.setCursor(((16 - text.length()) / 2), (line % 2));
  lcd.print(text);
}
