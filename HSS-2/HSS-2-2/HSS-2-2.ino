#include "rgb_lcd.h"
rgb_lcd lcd;
#define giris5V A1
#define giris12V A0
#define buton 2
#define buzzer A3
float voltaj = 0;
String hamVeri = ""; 
int gelenAci = 0;
int suankiKonum = 0;
bool kontrol = false;
unsigned long baslangic = 0;
#include <SoftwareSerial.h>

void setup() {
  pinMode(buton, INPUT);
  lcd.begin(16, 2);
  lcd.clear();  
  yaz("BMS SAVUNMA", 0);
  yaz("TEKNOLOJILERI", 1);
  delay(4000);
  lcd.clear();
  /*yaz("ANTEN TRAKER", 0);
  yaz("VERSIYON 1.0", 1);
  delay(4000);
  lcd.clear();
  yaz("BMS SAVUNMA TEK.", 0);
  yaz("V1      V2     ", 1);
  kontrol5V();
  kontrol12V();
  delay(4000);
  lcd.clear();
  yaz("ANTEN TRAKER", 0);
  yaz("KALIBRASYON", 1);
  for(int i = 0; i < 4; i++){ // buzzerCal isimli fonksiyon ile 4 saniye bekler.
    buzzerCal();
  }
  lcd.clear();
  kontrol = false;
  yazB("TERAZI", 0);
  metinlerArasiBekleme();
  while(true){ 
    yazB("Teraziyi duz", 1);
    metinlerArasiBekleme();
    yazB("olmasi icin", 1);
    metinlerArasiBekleme();
    yazB("ayarlayin.", 1);
    metinlerArasiBekleme();
    yazB("Duz olunca", 1);
    metinlerArasiBekleme(); 
    yazB("butona basin.", 1);
    metinlerArasiBekleme();
    baslangic = millis();
    while(millis() - baslangic <= 5000){
      if(digitalRead(buton)){
        for(int i = 0; i < 4; i++){
          buzzerCal();
        }
        kontrol = true;
        break;
      }
    }
    if(kontrol){
      break;  
    }
  }
  lcd.clear();
  kontrol = false;
  yazB("Y EKSENI", 0);
  metinlerArasiBekleme();
  while(true){
    yazB("Y Ekseni motoru", 1);
    metinlerArasiBekleme();
    yazB("0 dereceye gelip", 1);
    metinlerArasiBekleme();
    yazB("durdugunda zemin", 1);
    metinlerArasiBekleme();
    yazB("ile paralel ise", 1);
    metinlerArasiBekleme();
    yazB("butona basin.", 1);
    metinlerArasiBekleme();
    baslangic = millis();
    while(millis() - baslangic <= 5000){
      if(digitalRead(buton)){
        for(int i = 0; i < 4; i++){
          buzzerCal();
        }
        kontrol = true;
        break;
      }
    }
    if(kontrol){
      break;  
    }
  }
  lcd.clear();
  kontrol = false;
  yazB("X EKSENI", 0);
  metinlerArasiBekleme();
  while(true){
    yazB("Pusulayi", 1);
    metinlerArasiBekleme();
    yazB("yerlestirin.", 1);
    metinlerArasiBekleme();
    yazB("X Ekseni motoru", 1);
    metinlerArasiBekleme();
    yazB("0 dereceye gelip", 1);
    metinlerArasiBekleme();
    yazB("durdugunda 0", 1);
    metinlerArasiBekleme();
    yazB("dereceyi ucus", 1);
    metinlerArasiBekleme();
    yazB("planinizin en", 1);
    metinlerArasiBekleme();
    yazB("uzak kismina", 1);
    metinlerArasiBekleme();
    yazB("getirmek icin", 1);
    metinlerArasiBekleme();
    yazB("sehpayi donderin", 1);
    metinlerArasiBekleme();
    yazB("Teraziyi kontrol", 1);
    metinlerArasiBekleme();
    yazB("edin. Pusuladaki", 1);
    metinlerArasiBekleme();
    yazB("kuzeyle olan", 1);
    metinlerArasiBekleme();
    yazB("aci farkini", 1);
    metinlerArasiBekleme();
    yazB("PC'ye girin", 1);
    metinlerArasiBekleme();
    yazB("butona basin.", 1);
    metinlerArasiBekleme();
    baslangic = millis();
    while(millis() - baslangic <= 5000){
      if(digitalRead(buton)){
        for(int i = 0; i < 4; i++){
          buzzerCal();
        }
        kontrol = true;
        break;
      }
    }
    if(kontrol){
      break;  
    }
  }
  lcd.clear();
  yaz("KALIBRASYON", 0);
  yaz("TAMAMLANDI", 1);
  delay(4000);
  lcd.clear();*/
  Serial.begin(9600);
}

void loop() {
  while (Serial.available()) {
    char veriBirimi = (char)Serial.read(); 
    hamVeri += veriBirimi;
    if (veriBirimi == '\n') {
      veriIsle();
      hamVeri = "";
    } 
  }
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

void yaz(String yazi, byte satir){
  for(int j = 0; j < 16; j++){
    lcd.setCursor(j, (satir % 2));
    lcd.print(" ");
  }
  lcd.setCursor(((16 - yazi.length()) / 2), (satir % 2));
  lcd.print(yazi);
}

void yazB(String yazi, byte satir){
  for(int j = 0; j < 16; j++){
    lcd.setCursor(j, (satir % 2));
    lcd.print(" ");
  }
  buzzerCal();
  lcd.setCursor(((16 - yazi.length()) / 2), (satir % 2));
  lcd.print(yazi);
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
  lcd.print(voltaj, 2);
}

void kontrol5V(){
  voltaj = voltajHesapla5V(); 
  ekranaYazdir(voltaj, 3, 1);
  delay(200);
}

void kontrol12V(){
  voltaj = voltajHesapla12V(); 
  ekranaYazdir(voltaj, 11, 1);
  delay(200);
}

float voltajHesapla5V(){
  voltaj = analogRead(giris5V);
  voltaj = voltaj * 0.007088068; 
  return voltaj;
}

float voltajHesapla12V(){
  voltaj = analogRead(giris12V);
  voltaj = voltaj * 0.01762247; 
  return voltaj;
}

void buzzerCal(){
  digitalWrite(buzzer, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);  
  delay(500);
}

void metinlerArasiBekleme(){
  buzzerCal();
  buzzerCal();
}
