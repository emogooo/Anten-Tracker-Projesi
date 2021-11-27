#include "SM.h"
#include "rgb_lcd.h"
#include <SoftwareSerial.h>
#define giris5V A1
#define giris12V A0
#define buton 2
#define buzzer A3
#define SMXLimit1Pin 9
#define SMXLimit2Pin 10
#define SMYLimit1Pin 11
#define SMYLimit2Pin 12
#define SMXDirPin 8
#define SMXStepPin 7
#define SMYDirPin 6
#define SMYStepPin 5
rgb_lcd lcd;
SM stepMotorlar(SMXStepPin, SMXDirPin, SMYStepPin, SMYDirPin, SMXLimit1Pin, SMXLimit2Pin, SMYLimit1Pin, SMYLimit2Pin);
String hamVeri = "";
char veriBirimi;
float voltaj = 0;
int gidilecekDereceMotorX = 0;
int gidilecekDereceMotorY = 0;
int metinlerArasiBeklemeSuresi = 2000;
unsigned long baslangic = 0;
bool kontrol = false;

void setup() {
  pinMode(buton, INPUT);
  attachInterrupt(1, limitSwitchInterrupt, RISING);
  lcd.begin(16, 2);
  lcd.clear();  
  //baslangicKalibrasyon();
  Serial.begin(9600);
  stepMotorlar.hizAyarla(500,400);
  yaz(" X            Y ", 0);
  
}

void loop() {
  while (Serial.available()) {
    veriBirimi = (char)Serial.read(); 
    hamVeri += veriBirimi;
    if (veriBirimi == '\n') {
      veriIsle();
      yaz((String(gidilecekDereceMotorX) + "          " + String(gidilecekDereceMotorY)), 1);     
      stepMotorlar.git(gidilecekDereceMotorX, gidilecekDereceMotorY);
    } 
  }
}

void limitSwitchInterrupt(){

}

void veriIsle(){
  int noktaIndisi = 0;
  noktaIndisi = hamVeri.indexOf('.' , 1);
  gidilecekDereceMotorX = (hamVeri.substring(0,noktaIndisi)).toInt();
  gidilecekDereceMotorY = (hamVeri.substring((noktaIndisi + 1), (hamVeri.length() - 2))).toInt();
  hamVeri = "";
}

void aciYaz(int aci, int x, int y){
  lcd.setCursor(x, y);
  lcd.print(aci);
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
  ekranaYazdir(voltaj, 11, 1);
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
  delay(metinlerArasiBeklemeSuresi);
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
  yaz("ATLAMAK ICIN", 1);
  metinlerArasiBuzzerlaBekleme();
  lcd.clear();
  yaz("BUTONA", 0);
  yaz("BASIN", 1);
  kontrol = butonDinle(5);
  lcd.clear();
  if(!kontrol){
    standartKalibrasyon();
    yaz("KALIBRASYON", 0);
    yaz("TAMAMLANDI", 1);
  }else{
    yaz("KALIBRASYON", 0);
    yaz("ATLANDI", 1);
  }
  delay(metinlerArasiBeklemeSuresi);
  lcd.clear();
}
