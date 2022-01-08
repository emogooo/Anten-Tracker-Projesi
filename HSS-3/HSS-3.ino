#include "SM.h"
#include "rgb_lcd.h"
#include <SoftwareSerial.h>
#define giris5V A1
#define giris12V A0
#define buton 2
#define buzzer A3
#define SMXLimit1Pin 10
#define SMXLimit2Pin 9
#define SMYLimit1Pin 11
#define SMYLimit2Pin 12
#define SMXDirPin 6
#define SMXStepPin 5
#define SMYDirPin 8
#define SMYStepPin 7
#define kesmePinDegeri 1 // Kartta 3. pin, fakat sistem bunu 1 olarak okuyor.
rgb_lcd lcd;
SM stepMotorlar(SMXStepPin, SMXDirPin, SMYStepPin, SMYDirPin, SMXLimit1Pin, SMXLimit2Pin, SMYLimit1Pin, SMYLimit2Pin);
String hamVeri = "";
char veriBirimi;
int gidilecekDereceMotorX = 0;
int gidilecekDereceMotorY = 0;

void setup() {
  pinMode(buton, INPUT);
  //attachInterrupt(kesmePinDegeri, limitSwitchInterrupt, RISING);
  lcd.begin(16, 2);
  lcd.clear();  
  kalibrasyon();
  Serial.begin(9600);
  yaz("  X          Y  ", 0);
  
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
  if(digitalRead(SMXLimit1Pin) == HIGH){
    
  }else if(digitalRead(SMXLimit2Pin) == HIGH){
    
  }else if(digitalRead(SMYLimit1Pin) == HIGH){
    
  }else if(digitalRead(SMYLimit2Pin) == HIGH){
    
  }
}

void veriIsle(){
  int noktaIndisi = 0;
  noktaIndisi = hamVeri.indexOf('.' , 1);
  gidilecekDereceMotorX = (hamVeri.substring(0,noktaIndisi)).toInt();
  gidilecekDereceMotorY = (hamVeri.substring((noktaIndisi + 1), (hamVeri.length() - 2))).toInt();
  hamVeri = "";
}

void yaz(String yazi, byte satir){
  for(int i = 0; i < 16; i++){
    lcd.setCursor(i, (satir % 2));
    lcd.print(" ");
  }
  lcd.setCursor(((16 - yazi.length()) / 2), (satir % 2));
  lcd.print(yazi);
}

void voltajYazdir(float voltaj, int x, int y){
  lcd.setCursor(x, y);
  lcd.print(voltaj, 2);
}

float kontrol5V(){
  float voltaj5 = analogRead(giris5V);
  voltaj5 = voltaj5 * 0.007088068; 
  voltajYazdir(voltaj5, 3, 1);
  delay(200);
  return voltaj5;
}

float kontrol12V(){
  float voltaj12 = analogRead(giris12V);
  voltaj12 = voltaj12 * 0.01762247; 
  voltajYazdir(voltaj12, 11, 1);
  delay(200);
  return voltaj12;
}

bool butonDinle(int saniye){
  unsigned long baslangic = millis();
  while(millis() - baslangic <= saniye * 1000){
        if(digitalRead(buton)){
          buzzerOnay();
          return true;
        }
  }
  return false;
}

void buzzerCal(){
  digitalWrite(buzzer, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);  
  delay(500);
}
void buzzerOnay(){
  digitalWrite(buzzer, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(1000);
}

void metinlerArasiBuzzerlaBekleme(){
  for(int i = 0; i < 2; i++){
    buzzerCal();
  }
}

void kalibrasyon(){
  bool butonKontrol = false;
  int metinlerArasiBeklemeSuresi = 2000;
  yaz("BMS SAVUNMA", 0);
  yaz("TEKNOLOJILERI", 1);
  delay(metinlerArasiBeklemeSuresi * 1.5);
  lcd.clear();
  yaz("ANTEN TRAKER", 0);
  yaz("VERSIYON 1.0", 1);
  delay(metinlerArasiBeklemeSuresi * 1.5);
  lcd.clear();
  yaz("BMS SAVUNMA TEK.", 0);
  yaz("V1      V2     ", 1);
  float voltaj5 = kontrol5V();
  float voltaj12 = kontrol12V();
  delay(metinlerArasiBeklemeSuresi * 1.5);
  lcd.clear();
  if((voltaj12 > 10.5 && voltaj12 < 14) && (voltaj5 > 4.5 && voltaj5 < 5.3)){
      yaz("BMS SAVUNMA TEK.", 0);
      yaz("VOLTAJLAR UYGUN", 1);
      delay(metinlerArasiBeklemeSuresi);
      buzzerOnay();
      lcd.clear();
  }else{
      yaz("BMS SAVUNMA TEK.", 0);
      yaz("VOLTAJ ARIZASI", 1);
      while(true){
        digitalWrite(buzzer, HIGH);
        delay(250);
        digitalWrite(buzzer, LOW);  
        delay(250);
      }      
  }
  yaz("ANTEN TRAKER", 0);
  yaz("KALIBRASYON", 1);
  metinlerArasiBuzzerlaBekleme();
  lcd.clear();
  yaz("Y EKSENI", 0);
  buzzerCal();
  yaz("KALIBRASYON", 1);
  buzzerCal();
  metinlerArasiBuzzerlaBekleme();
  stepMotorlar.yAdimSay();
  buzzerOnay();
  lcd.clear();
  yaz("X EKSENI", 0);
  buzzerCal();
  yaz("KALIBRASYON", 1);
  buzzerCal();
  metinlerArasiBuzzerlaBekleme();
  stepMotorlar.xAdimSay();
  while(true){
    yaz("Anteni IHA'ya", 1);
    buzzerCal();
    metinlerArasiBuzzerlaBekleme();
    yaz("cevirin.", 1);
    buzzerCal();
    metinlerArasiBuzzerlaBekleme();
    yaz("Pusuladaki", 1);
    buzzerCal();
    metinlerArasiBuzzerlaBekleme();
    yaz("kuzeyle olan aci", 1);
    buzzerCal();
    metinlerArasiBuzzerlaBekleme();
    yaz("farkini PC'ye", 1);
    buzzerCal();
    metinlerArasiBuzzerlaBekleme();
    yaz("girin ve", 1);
    buzzerCal();
    metinlerArasiBuzzerlaBekleme();
    yaz("butona basin.", 1);
    buzzerCal();
    metinlerArasiBuzzerlaBekleme();
    butonKontrol = butonDinle(5);
    if(butonKontrol){
      break;  
    }
  }
  delay(metinlerArasiBeklemeSuresi);
  lcd.clear();
  yaz("KALIBRASYON", 0);
  yaz("TAMAMLANDI", 1);
  delay(metinlerArasiBeklemeSuresi);
  buzzerOnay();
  lcd.clear();
}
