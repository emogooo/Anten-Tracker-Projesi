#include "rgb_lcd.h"
rgb_lcd lcd;
#define giris5V A1
#define giris24V A0
#define buton 2
float voltaj = 0;
String hamVeri = ""; 
int gelenAci = 0;
int suankiKonum = 0;
int metinlerArasiBekleme = 1000;

void setup() {
  pinMode(buton, INPUT);
  lcd.begin(16, 2);
  lcd.clear();  
  yaz("BMS SAVUNMA", 0);
  yaz("TEKNOLOJILERI", 1);
  delay(4000);
  lcd.clear();
  yaz("ANTEN TRAKER", 0);
  yaz("VERSIYON 1.0", 1);
  delay(4000);
  lcd.clear();
  yaz("BMS SAVUNMA TEK.", 0);
  yaz("V1      V2     ", 1);
  /*while(true){
    kontrol5V();
    kontrol24V();
  }*/
  
  delay(4000);
  lcd.clear();
  yaz("ANTEN TRAKER", 0);
  yaz("KALIBRASYON", 1);
  for(int i = 0; i < 4; i++){ // buzzerCal isimli fonksiyon ile 4 saniye bekler.
    buzzerCal();
  }
  delay(4000);
  lcd.clear();
  yaz("TERAZI", 0);
  delay(metinlerArasiBekleme);
  yaz("Teraziyi duz", 1);
  delay(metinlerArasiBekleme);
  yaz("olmasi icin", 1);
  delay(metinlerArasiBekleme);
  yaz("ayarlayin. Duz", 1);
  delay(metinlerArasiBekleme);
  yaz("olunca butona", 1);
  delay(metinlerArasiBekleme);
  yaz("basin.", 1);
  while(!digitalRead(buton));
  for(int i = 0; i < 4; i++){ // Burada buton beklenecek
    buzzerCal();
  }
  lcd.clear();
  yaz("Y EKSENI", 0);
  delay(metinlerArasiBekleme);
  yaz("Y Ekseni motoru", 1);
  delay(metinlerArasiBekleme);
  yaz("0 dereceye gelip", 1);
  delay(metinlerArasiBekleme);
  yaz("durdugunda zemin", 1);
  delay(metinlerArasiBekleme);
  yaz("ile paralel ise", 1);
  delay(metinlerArasiBekleme);
  yaz("butona basin.", 1);
  while(!digitalRead(buton));
  for(int i = 0; i < 4; i++){ // Burada buton beklenecek
    buzzerCal();
  }
  lcd.clear();
  yaz("X EKSENI", 0);
  delay(metinlerArasiBekleme);
  yaz("Pusulayi", 1);
  delay(metinlerArasiBekleme);
  yaz("yerlestirin.", 1);
  delay(metinlerArasiBekleme);
  yaz("X Ekseni motoru", 1);
  delay(metinlerArasiBekleme);
  yaz("0 dereceye gelip", 1);
  delay(metinlerArasiBekleme);
  yaz("durdugunda 0", 1);
  delay(metinlerArasiBekleme);
  yaz("dereceyi ucus", 1);
  delay(metinlerArasiBekleme);
  yaz("planinizin en", 1);
  delay(metinlerArasiBekleme);
  yaz("uzak kismina", 1);
  delay(metinlerArasiBekleme);
  yaz("getirmek icin", 1);
  delay(metinlerArasiBekleme);
  yaz("sehpayi donderin", 1);
  delay(metinlerArasiBekleme);
  yaz("Teraziyi kontrol", 1);
  delay(metinlerArasiBekleme);
  yaz("edin. Pusuladaki", 1);
  delay(metinlerArasiBekleme);
  yaz("kuzeyle olan aci", 1);
  delay(metinlerArasiBekleme);
  yaz("farkini PC'ye", 1);
  delay(metinlerArasiBekleme);
  yaz("girin butona", 1);
  delay(metinlerArasiBekleme);
  yaz("basin.", 1);
  while(!digitalRead(buton));
  for(int i = 0; i < 4; i++){ // Burada buton beklenecek
    buzzerCal();
  }
  lcd.clear();
  yaz("KALIBRASYON", 0);
  yaz("TAMAMLANDI", 1);
  delay(4000);
  lcd.clear();
  Serial.begin(9600);
}

void loop() {
  /*while (Serial.available()) {
    char veriBirimi = (char)Serial.read(); 
    hamVeri += veriBirimi;
    if (veriBirimi == '\n') {
      veriIsle();
      hamVeri = "";
      if(gelenAci != suankiKonum){ 
        int hareketDerecesi = gelenAci - suankiKonum;
        hareketDerecesi = hareketDerecesi * 1.11111;
        //s.move(hareketDerecesi);
        //s.runToPosition();
        suankiKonum = gelenAci;
      }
    } 
  }*/
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

// 16 harften kısa olan yazıları ortalayıp; uzunları ise sola kaydırarak görüntüler.
void yaz(String yazi, byte satir){
  if(yazi.length() <= 16){
    for(int j = 0; j < 16; j++){
      lcd.setCursor(j, (satir % 2));
      lcd.print(" ");
    }
    lcd.setCursor(((16 - yazi.length()) / 2), (satir % 2));
    lcd.print(yazi);
  }else{
    for(int i = 0; i < yazi.length() - 14; i += 2){
      for(int j = 0; j < 16; j++){
        lcd.setCursor(j, (satir % 2));
        lcd.print(" ");
      }
      lcd.setCursor(0, (satir % 2));
      lcd.print(yazi.substring(i, (i+16)));
      delay(400);
    }   
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

void ekranaYazdir(float voltaj, int x, int y){
  lcd.setCursor(x, y);
  lcd.print(voltaj);
}

void kontrol5V(){
  voltaj = voltajHesapla5V(); 
  ekranaYazdir(voltaj, 3, 1);
  delay(200);
}

void kontrol24V(){
  voltaj = voltajHesapla24V(); 
  ekranaYazdir(voltaj, 11, 1);
  delay(200);
}

float voltajHesapla5V(){
  voltaj = analogRead(giris5V);
  voltaj = voltaj * 0.005859; 
  return voltaj;
}

float voltajHesapla24V(){
  voltaj = analogRead(giris24V);
  voltaj = voltaj * 0.0166; 
  return voltaj;
}

void buzzerCal(){
  /*digitalWrite(x, HIGH);
  delay(500);
  digitalWrite(x, LOW);  
  delay(500);*/
}
