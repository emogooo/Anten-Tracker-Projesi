#include "rgb_lcd.h"
#include <SoftwareSerial.h>
#include <AccelStepper.h>
#define giris5V A1
#define giris12V A0
#define buton 2
#define buzzer A3
#define d9 9
#define d10 10
#define d11 11
#define d12 12
rgb_lcd lcd;
AccelStepper motor1(1, 7, 8); // Motor Arayüz Tipi, Step Pini, Yön Pini
AccelStepper motor2(1, 5, 6); // Motor Arayüz Tipi, Step Pini, Yön Pini
String hamVeri = ""; ,
char veriBirimi = '';
float voltaj = 0;
int gidilecekAciMotor1 = 0;
int gidilecekAciMotor2 = 0;
int konumMotor1 = 0;
int konumMotor2 = 0;
int metinlerArasiBeklemeSuresi = 2000;
int int hareketDerecesi1 = 0;
int hareketDerecesi2 = 0;
bool kontrol = false;
unsigned long baslangic = 0;

void setup() {
  motor1.setAcceleration(8000);
  motor2.setAcceleration(10000);
  pinMode(buton, INPUT);
  attachInterrupt(1, limitSwitchInterrupt, RISING);
  lcd.begin(16, 2);
  lcd.clear();  
  baslangicKalibrasyon();
  Serial.begin(9600);
}

void loop() {
  while (Serial.available()) {
    veriBirimi = (char)Serial.read(); 
    hamVeri += veriBirimi;
    if (veriBirimi == '\n') {
      veriIsle();
      lcd.clear();
      ekranaYazdir(gidilecekAciMotor1, 0, 0);
      ekranaYazdir(gidilecekAciMotor2, 13, 0);      
      if(gidilecekAciMotor1 != konumMotor1){ 
        hareketDerecesi1 = gidilecekAciMotor1 - konumMotor1;
        hareketDerecesi1 = hareketDerecesi1 * 58;
        motor1.move(hareketDerecesi1);
        motor1.runToPosition();
        konumMotor1 = gidilecekAciMotor1;
      }
      if(gidilecekAciMotor2 != konumMotor2){ 
        hareketDerecesi2 = gidilecekAciMotor2 - konumMotor2;
        hareketDerecesi2 = -hareketDerecesi2 * 55;
        motor2.move(hareketDerecesi2);
        motor2.runToPosition();
        konumMotor2 = gidilecekAciMotor2;
      }
    } 
  }
}

void limitSwitchInterrupt(){

}

void veriIsle(){
  int noktaIndisi = 0;
  noktaIndisi = hamVeri.indexOf('.' , 1);
  gidilecekAciMotor1 = (hamVeri.substring(0,noktaIndisi)).toInt();
  gidilecekAciMotor2 = (hamVeri.substring((noktaIndisi + 1), (hamVeri.length() - 2))).toInt();
  hamVeri = "";
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

void ekranaYazdir(float voltaj, int x, int y){
  lcd.setCursor(x, y);
  lcd.print(voltaj, 2);
}

void kontrol5V(){
  voltaj = analogRead(giris5V);
  voltaj = voltaj * 0.007088068; 
  ekranaYazdir(voltaj, 3, 1);
  delay(200);
}

void kontrol12V(){
  voltaj = analogRead(giris12V);
  voltaj = voltaj * 0.01762247; 
  delay(200);
}

bool butonDinle(int saniye){
  baslangic = millis();
  while(millis() - baslangic <= saniye * 1000){
        if(digitalRead(buton)){
          buzzerButon();
          return true;
        }
  }
  return false;
}

void butonBasGec(){
  while(!digitalRead(buton));
  buzzerButon();
}

void buzzerCal(){
  digitalWrite(buzzer, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);  
  delay(500);
}

void buzzerButon(){
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);  
}

void metinlerArasiBuzzerlaBekleme(){
  for(int i = 0; i < 2; i++){
    buzzerCal();
  }
}

void baslangicKalibrasyonGiris(){
  yaz("BMS SAVUNMA", 0);
  yaz("TEKNOLOJILERI", 1);
  delay(metinlerArasiBeklemeSuresi);
  lcd.clear();
  yaz("ANTEN TRAKER", 0);
  yaz("VERSIYON 1.0", 1);
  delay(metinlerArasiBeklemeSuresi);
  lcd.clear();
  yaz("BMS SAVUNMA TEK.", 0);
  yaz("V1      V2     ", 1);
  kontrol5V();
  kontrol12V();
  delay(metinlerArasiBeklemeSuresi);
  lcd.clear();
  yaz("ANTEN TRAKER", 0);
  yaz("KALIBRASYON", 1);
  metinlerArasiBuzzerlaBekleme();
  lcd.clear();
}

void hizliKalibrasyon(){
  yaz("TERAZI ICIN", 0); 
  yaz("BUTONA BASIN.", 1);
  metinlerArasiBuzzerlaBekleme();
  butonBasGec();
  lcd.clear();
  yaz("Y EKSENI ICIN", 0);
  yaz("BUTONA BASIN.", 1);
  metinlerArasiBuzzerlaBekleme();
  butonBasGec();
  lcd.clear();
  yaz("X EKSENI ICIN", 0);
  yaz("BUTONA BASIN.", 1);
  metinlerArasiBuzzerlaBekleme();
  butonBasGec();
  lcd.clear();
}

void standartKalibrasyon(){
  yazB("TERAZI", 0);
  metinlerArasiBuzzerlaBekleme();
  while(true){ 
    yazB("Teraziyi duz", 1);
    metinlerArasiBuzzerlaBekleme();
    yazB("olmasi icin", 1);
    metinlerArasiBuzzerlaBekleme();
    yazB("ayarlayin.", 1);
    metinlerArasiBuzzerlaBekleme();
    yazB("Duz olunca", 1);
    metinlerArasiBuzzerlaBekleme(); 
    yazB("butona basin.", 1);
    metinlerArasiBuzzerlaBekleme();
    kontrol = butonDinle(5);
    if(kontrol){
      break;  
    }
  }
  lcd.clear();
  yazB("Y EKSENI", 0);
  metinlerArasiBuzzerlaBekleme();
  while(true){
    yazB("Y Ekseni motoru", 1);
    metinlerArasiBuzzerlaBekleme();
    yazB("0 dereceye gelip", 1);
    metinlerArasiBuzzerlaBekleme();
    yazB("durdugunda zemin", 1);
    metinlerArasiBuzzerlaBekleme();
    yazB("ile paralel ise", 1);
    metinlerArasiBuzzerlaBekleme();
    yazB("butona basin.", 1);
    metinlerArasiBuzzerlaBekleme();
    kontrol = butonDinle(5);
    if(kontrol){
      break;  
    }
  }
  lcd.clear();
  yazB("X EKSENI", 0);
  metinlerArasiBuzzerlaBekleme();
  while(true){
    yazB("Pusulayi", 1);
    metinlerArasiBuzzerlaBekleme();
    yazB("yerlestirin.", 1);
    metinlerArasiBuzzerlaBekleme();
    yazB("X Ekseni motoru", 1);
    metinlerArasiBuzzerlaBekleme();
    yazB("0 dereceye gelip", 1);
    metinlerArasiBuzzerlaBekleme();
    yazB("durdugunda 0", 1);
    metinlerArasiBuzzerlaBekleme();
    yazB("dereceyi ucus", 1);
    metinlerArasiBuzzerlaBekleme();
    yazB("planinizin en", 1);
    metinlerArasiBuzzerlaBekleme();
    yazB("uzak kismina", 1);
    metinlerArasiBuzzerlaBekleme();
    yazB("getirmek icin", 1);
    metinlerArasiBuzzerlaBekleme();
    yazB("sehpayi donderin", 1);
    metinlerArasiBuzzerlaBekleme();
    yazB("Teraziyi kontrol", 1);
    metinlerArasiBuzzerlaBekleme();
    yazB("edin. Pusuladaki", 1);
    metinlerArasiBuzzerlaBekleme();
    yazB("kuzeyle olan", 1);
    metinlerArasiBuzzerlaBekleme();
    yazB("aci farkini", 1);
    metinlerArasiBuzzerlaBekleme();
    yazB("PC'ye girin", 1);
    metinlerArasiBuzzerlaBekleme();
    yazB("butona basin.", 1);
    metinlerArasiBuzzerlaBekleme();
    kontrol = butonDinle(5);
    if(kontrol){
      break;  
    }
  }
  lcd.clear();
}

void baslangicKalibrasyon(){ 
  baslangicKalibrasyonGiris(); 
  yaz("KALIBRASYONU", 0);
  yaz("GECMEK ICIN", 1);
  metinlerArasiBuzzerlaBekleme();
  lcd.clear();
  yaz("BUTONA", 0);
  yaz("BASIN", 1);
  kontrol = butonDinle(5);
  lcd.clear();
  if(!kontrol){
    yaz("HIZLI", 0);
    yaz("KALIBRASYON ICIN", 1);
    metinlerArasiBuzzerlaBekleme();
    lcd.clear();
    yaz("BUTONA", 0);
    yaz("BASIN", 1);
    kontrol = butonDinle(5);  
    lcd.clear();
    if(kontrol){
      hizliKalibrasyon();
    }
    else{
      standartKalibrasyon();
    }
  }
  yaz("KALIBRASYON", 0);
  yaz("TAMAMLANDI", 1);
  delay(metinlerArasiBeklemeSuresi);
  lcd.clear();
}
