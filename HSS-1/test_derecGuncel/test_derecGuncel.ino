#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 6, 5, 4, 3); 
#define Buton 8
#define x A0
#define y A1
int degerx  = 0;
int degery  = 0;

void setup(){
  Serial.begin(9600);
  lcd.begin(16, 2); 
  baslangicEkraniniGoster();
}

void loop(){
  delay(80);
  degerx  = analogRead(x);
  degery  = analogRead(y);
  degerx = 0.35156 * degerx; 
  degery = 0.21972 * degery; 
  lcd.clear();
  ekranaYazdir(degerx, 13, 0);
  ekranaYazdir(degery, 0, 0);
  lcd.setCursor(5, 0);
  lcd.print("derece");
  if (digitalRead(Buton) == 1){
    Serial.print(degerx);
    Serial.print("."); // BURADA SERİ HABERLEŞME VAR...!!!??? Gönderiyor
    Serial.println(degery);
    lcd.setCursor(3, 1);
    lcd.print(" Transmit ");
  } 
}

void ekranaYazdir(int aci, int x, int y){
  lcd.setCursor(x, y);
  lcd.print(aci);
}

void ekranaYazdir(String mesaj, int x, int y){
  lcd.setCursor(x, y);
  lcd.print(mesaj);
}

void baslangicEkraniniGoster(){
  ekranaYazdir("TEKNOMER", 4, 0);
  ekranaYazdir("ANTEN TRACER", 2, 1);
  delay(4000);
  lcd.clear();
}
