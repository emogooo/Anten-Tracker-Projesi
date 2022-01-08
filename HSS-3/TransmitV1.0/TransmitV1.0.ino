#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 6, 5, 4, 3); 
#define Buton 8
#define x A0
#define y A1
int degerX  = 0;
int degerY  = 0;

void setup(){
  lcd.begin(16, 2);
  lcd.clear();
  yaz("BMS", 0);
  yaz("ANTEN TRACKER", 1);
  delay(2000);
  lcd.clear();
  Serial.begin(9600);
  yaz("   Y        X   ", 0);
}

void loop(){
  delay(100);
  degerX  = analogRead(x);
  degerY  = analogRead(y);
  degerX = 0.34179 * degerX; 
  degerY = 0.08789 * degerY; 
  xYazdir(degerX);
  yYazdir(degerY);
  if (digitalRead(Buton) == 1){
    Serial.print(degerX);
    Serial.print(".");
    Serial.println(degerY);
    lcd.setCursor(7, 0);
    lcd.print("+");
    delay(250);
    lcd.setCursor(7, 0);
    lcd.print(" ");
  } 
}

void xYazdir(int aci){
  for(int i = 11; i < 14; i++){
    lcd.setCursor(i, 1);
    lcd.print(" ");
  }
  if(aci < 10){
    lcd.setCursor(12, 1); 
  }else{
    lcd.setCursor(11, 1);
  }
  lcd.print(aci);
}

void yYazdir(int aci){
  for(int i = 2; i < 4; i++){
    lcd.setCursor(i, 1);
    lcd.print(" ");
  }
  if(aci < 10){
    lcd.setCursor(3, 1); 
  }else{
    lcd.setCursor(2, 1);
  }
  lcd.print(aci);
}

void yaz(String yazi, byte satir){
  for(int i = 0; i < 16; i++){
    lcd.setCursor(i, (satir % 2));
    lcd.print(" ");
  }
  lcd.setCursor(((16 - yazi.length()) / 2), (satir % 2));
  lcd.print(yazi);
}
