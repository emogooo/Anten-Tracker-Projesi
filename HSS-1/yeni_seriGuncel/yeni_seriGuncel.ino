#include <LiquidCrystal.h>
LiquidCrystal lcd(41, 40, 34, 33, 32, 31);
#define giris5V A0
#define giris12V A1
#define giris60V 52
#define giris24V 53
#define SM225PWM 12
#define SM225Yon 47
#define SM225Fren 23
#define SM225LimitButonu1 0
#define SM225LimitButonu2 4
// 19. ve 20. pinler limit butonları. 19:4, 2:0 dış kesme
#define SM360PWM 13
#define SM360Yon 25
#define SM360Fren 45
#define SM360LimitButonu 5
// 18. pin limit butonu. 18:5, dış kesme
#define acilDurumButonu 2
// 21. pin limit butonu. 21:2, dış kesme
float voltaj = 0; 
String hamVeri = "";
volatile byte SM360YonBilgisi = HIGH;
volatile unsigned int SM360LBSonCalismaZamani = 0;

void setup() {
  ilklendir();
  baslangicKontrol();
  baslangicEkraniniGoster();
  SM225Calistir();
  SM360Calistir();
  Serial3.begin(9600);
}

void loop() {
  while (Serial3.available()) {
    char veriBirimi = (char)Serial3.read(); 
    hamVeri += veriBirimi;
    if (veriBirimi == '\n') {
      veriIsle();
      hamVeri = "";
    } 
  }
}

void ilklendir(){
  pinMode(giris5V, INPUT);
  pinMode(giris12V, INPUT);
  pinMode(giris24V, INPUT);
  pinMode(giris60V, INPUT);
  pinMode(SM225PWM, OUTPUT);
  pinMode(SM225Yon, OUTPUT);
  pinMode(SM225Fren, OUTPUT);
  pinMode(SM360PWM, OUTPUT);
  pinMode(SM360Yon, OUTPUT);
  pinMode(SM360Fren, OUTPUT);
  attachInterrupt(SM225LimitButonu1, SM225YonDegistir1, RISING);
  attachInterrupt(SM225LimitButonu2, SM225YonDegistir2, RISING);
  attachInterrupt(SM360LimitButonu, SM360YonDegistir, RISING);
  attachInterrupt(acilDurumButonu, motorlariDurdur, RISING);
  lcd.begin(16, 1);
}

void baslangicKontrol(){
  ekranaYazdir("SISTEM KONTROLU", 0, 0);
  delay(2000);
  lcd.clear();
  kontrol24V();
  kontrol60V();
  kontrol5V();
  kontrol12V();
  ekranaYazdir("SISTEM BASLIYOR", 0, 0);
  delay(1500);
  lcd.clear();
}

void baslangicEkraniniGoster(){
  ekranaYazdir("TEKNOMER", 4, 0);
  delay(2000);
  ekranaYazdir("ANTEN TRACER", 2, 0);
  delay(4000);
  lcd.clear();
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

void veriIsle(){
  int noktaIndisi = 0;
  noktaIndisi = hamVeri.indexOf('.' , 1);
  int ilkAci = (hamVeri.substring(0,noktaIndisi)).toInt();
  int ikinciAci = (hamVeri.substring((noktaIndisi + 1), (hamVeri.length() - 2))).toInt();
  lcd.clear();
  ekranaYazdir(ilkAci, 13, 0);
  ekranaYazdir(ikinciAci, 0, 0);
}

void kontrol24V(){
  ekranaYazdir("24V", 3, 0);
  delay(1000);
  if(digitalRead(giris24V) == LOW){
    ekranaYazdir("-", 11, 0);
    while(digitalRead(giris24V) == LOW);
  }
  ekranaYazdir("+", 11, 0);
  delay(2000);
  lcd.clear();
}

void kontrol60V(){
  ekranaYazdir("60V", 3, 0);
  delay(1000);
  if(digitalRead(giris60V) == LOW){
    ekranaYazdir("-", 11, 0);
    while(digitalRead(giris60V) == LOW);
  }
  ekranaYazdir("+", 11, 0);
  delay(2000);
  lcd.clear();
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

void kontrol12V(){
  ekranaYazdir("12V", 1, 0);
  delay(1000);
  while(true){
    voltaj = voltajHesapla12V(); 
    ekranaYazdir(voltaj, 6, 0);
    ekranaYazdir("12V", 1, 0);
    ekranaYazdir("-", 14, 0);
    ekranaYazdir("V", 11, 0);
     if(voltaj > 11.75 && voltaj < 12.25){
        ekranaYazdir("+", 14, 0);
        delay(2000);
        break;
     }
     delay(200);
     lcd.clear();
  }
  lcd.clear();
}

float voltajHesapla5V(){
  voltaj = analogRead(giris5V);
  voltaj = (6.1 * voltaj) / 1024;
  return voltaj;
}

float voltajHesapla12V(){
  voltaj = analogRead(giris12V);
  voltaj = (15.65 * voltaj) / 1024;
  return voltaj;
}

void SM225Calistir(){
  digitalWrite(SM225Fren, HIGH); //Freni açıyoruz.
  digitalWrite(SM225Yon, HIGH);
  analogWrite(SM225PWM, 100);
}

void SM360Calistir(){
  digitalWrite(SM360Fren, HIGH); //Freni açıyoruz.
  digitalWrite(SM360Yon, SM360YonBilgisi);
  analogWrite(SM360PWM, 100);
}

void SM225YonDegistir1(){
  digitalWrite(SM225Yon, HIGH);
}

void SM225YonDegistir2(){
  digitalWrite(SM225Yon, LOW);
}

void SM360YonDegistir(){
  if((millis() - SM360LBSonCalismaZamani) > 2000) { // Çalıştıktan sonra 2 saniye durur.
    SM360YonBilgisi = !SM360YonBilgisi;
    digitalWrite(SM360Yon, SM360YonBilgisi);
    SM360LBSonCalismaZamani = millis();
  }
}

void motorlariDurdur(){
  analogWrite(SM225PWM, 0);
  analogWrite(SM360PWM, 0);
  digitalWrite(SM225Fren, LOW);
  digitalWrite(SM360Fren, LOW);
}
