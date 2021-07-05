#include <AccelStepper.h>
AccelStepper s(1, 53, 52); // Motor Arayüz Tipi, Step Pini, Yön Pini
#include <LiquidCrystal.h>
LiquidCrystal lcd(41, 40, 34, 33, 32, 31);
#define giris5V A1
#define giris24V A0
float voltaj = 0;
String hamVeri = ""; 
int gelenAci = 0;
int suankiKonum = 0;

void setup() {
  ilklendir();
  baslangicKontrol();
  baslangicEkraniniGoster();
  Serial3.begin(9600);
}

void loop() {
 while (Serial3.available()) {
    char veriBirimi = (char)Serial3.read(); 
    hamVeri += veriBirimi;
    if (veriBirimi == '\n') {
      veriIsle();
      hamVeri = "";
      if(gelenAci != suankiKonum){ 
        int hareketDerecesi = gelenAci - suankiKonum;
        hareketDerecesi = hareketDerecesi * 1.11111;
        s.move(hareketDerecesi);
        s.runToPosition();
        suankiKonum = gelenAci;
      }
    } 
  }
}

void ilklendir(){
  pinMode(giris5V, INPUT);
  pinMode(giris24V, INPUT);
  s.setMaxSpeed(500);
  s.setAcceleration(100);
  s.setSpeed(100);
  s.moveTo(200);
  lcd.begin(16, 2);
}

void baslangicKontrol(){
  ekranaYazdir("SISTEM KONTROLU", 0, 0);
  delay(2000);
  lcd.clear();
  kontrol5V();
  kontrol24V();
  ekranaYazdir("SISTEM BASLIYOR", 0, 0);
  delay(1500);
  lcd.clear();
}

void baslangicEkraniniGoster(){
  ekranaYazdir("TEKNOMER", 4, 0);
  delay(2000);
  ekranaYazdir("ANTEN TRACER", 2, 1);
  delay(4000);
  lcd.clear();
}

void veriIsle(){
  int noktaIndisi = 0;
  noktaIndisi = hamVeri.indexOf('.' , 1);
  gelenAci = (hamVeri.substring(0,noktaIndisi)).toInt();
  int gelenAci2 = (hamVeri.substring((noktaIndisi + 1), (hamVeri.length() - 2))).toInt();
  lcd.clear();
  ekranaYazdir(gelenAci2, 13, 0);
  ekranaYazdir(gelenAci, 0, 0);
}

void ekranaYazdir(int aci, int x, int y){
  lcd.setCursor(x, y);
  lcd.print(aci);
}

void ekranaYazdir(String mesaj, int x, int y){
  lcd.setCursor(x, y);
  lcd.print(mesaj);
}

void ekranaYazdir(float voltaj, int x, int y){
  lcd.setCursor(x, y);
  lcd.print(voltaj);
}

void kontrol5V(){
  ekranaYazdir("5V", 2, 0);
  delay(1000);
  ekranaYazdir("-", 13, 0);
  ekranaYazdir("V", 10, 0);
  while(true){
    voltaj = voltajHesapla5V(); 
    ekranaYazdir(voltaj, 6, 0);
     if(voltaj > 4.75 && voltaj < 5.25){
        ekranaYazdir("+", 13, 0);
        delay(2000);
        break;
     }
     delay(200);
  }
  lcd.clear();
}

void kontrol24V(){
  ekranaYazdir("24V", 2, 0);
  delay(1000);
  ekranaYazdir("-", 13, 0);
  ekranaYazdir("V", 10, 0);
  while(true){
    voltaj = voltajHesapla24V(); 
    ekranaYazdir(voltaj, 6, 0);
     if(voltaj > 22 && voltaj < 26){
        ekranaYazdir("+", 13, 0);
        delay(2000);
        break;
     }
     delay(200);
  }
  lcd.clear();
}

float voltajHesapla5V(){
  voltaj = analogRead(giris5V);
  voltaj = voltaj * 0.007173; 
  return voltaj;
}

float voltajHesapla24V(){
  voltaj = analogRead(giris24V);
  voltaj = voltaj * 0.02765; 
  return voltaj;
}
