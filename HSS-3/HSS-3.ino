#include "SM.h"
#include "rgb_lcd.h"
#include <SoftwareSerial.h>
#define giris5V A1
#define giris12V A0
#define buton 2
#define buzzer A3
#define SMX350DereceLimitPin 10
#define SMX0DereceLimitPin 9
#define SMY90DereceLimitPin 11
#define SMY0DereceLimitPin 12
#define SMXDirPin 6
#define SMXStepPin 5
#define SMYDirPin 8
#define SMYStepPin 7
#define kesmePinDegeri 1 // Kartta 3. pin, fakat sistem bunu 1 olarak okuyor.
rgb_lcd lcd;
SM stepMotorlar(SMXStepPin, SMXDirPin, SMYStepPin, SMYDirPin, SMX350DereceLimitPin, SMX0DereceLimitPin, SMY90DereceLimitPin, SMY0DereceLimitPin);
String hamVeri = "";
char veriBirimi;
unsigned int gidilecekDereceMotorX = 0;
unsigned int gidilecekDereceMotorY = 0;

void setup() {
  pinMode(buton, INPUT); 
  lcd.begin(16, 2);
  lcd.clear();  
  kalibrasyon();
  stepMotorlar.hizAyarla(300, 300);
  //attachInterrupt(kesmePinDegeri, limitKesmeFonksiyonu, RISING);
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

void limitKesmeFonksiyonu(){
  if(digitalRead(SMX350DereceLimitPin) == HIGH){
    
  }else if(digitalRead(SMX0DereceLimitPin) == HIGH){
    
  }else if(digitalRead(SMY90DereceLimitPin) == HIGH){
    
  }else if(digitalRead(SMY0DereceLimitPin) == HIGH){
    
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

float kontrol5V(){
  float voltaj = analogRead(giris5V);
  voltaj = voltaj * 0.007088068; 
  lcd.setCursor(3, 1);
  lcd.print(voltaj, 2);
  return voltaj;
}

float kontrol12V(){
  float voltaj = analogRead(giris12V);
  voltaj = voltaj * 0.01762247; 
  lcd.setCursor(11, 1);
  lcd.print(voltaj, 2);
  return voltaj;
}

bool butonDinle(unsigned int saniye){
  unsigned long baslangic = millis();
  while(millis() - baslangic <= saniye * 1000){
        if(digitalRead(buton)){
          buzzerOlumlu();
          return true;
        }
  }
  return false;
}

void buzzerCal(unsigned int ms, unsigned int adet){
  for(unsigned int i = 0; i < adet; i++){
    digitalWrite(buzzer, HIGH);
    delay(ms);
    digitalWrite(buzzer, LOW);  
    delay(ms);
  }
}

void buzzerOlumlu(){
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

void kalibrasyon(){
  int metinlerArasiBeklemeSuresi = 3000;
  yaz("BMS SAVUNMA", 0);
  yaz("TEKNOLOJILERI", 1);
  delay(metinlerArasiBeklemeSuresi);
  yaz("ANTEN TRAKER", 0);
  yaz("VERSIYON 1.0", 1);
  delay(metinlerArasiBeklemeSuresi);
  yaz("BMS SAVUNMA TEK.", 0);
  yaz("V1      V2     ", 1);
  float voltaj5 = kontrol5V();
  float voltaj12 = kontrol12V();
  delay(metinlerArasiBeklemeSuresi);
  if((voltaj12 > 10.5 && voltaj12 < 14) && (voltaj5 > 4.5 && voltaj5 < 5.3)){
      yaz("BMS SAVUNMA TEK.", 0);
      yaz("VOLTAJLAR UYGUN", 1);
      delay(metinlerArasiBeklemeSuresi);
      buzzerOlumlu();
  }else{
      yaz("BMS SAVUNMA TEK.", 0);
      yaz("VOLTAJ ARIZASI", 1);
      while(true){
        buzzerCal(250, 1);
      }      
  }
  yaz("ANTEN TRAKER", 0);
  yaz("KALIBRASYON", 1);
  buzzerCal(500,3);
  yaz("Y EKSENI", 0);
  yaz("KALIBRASYON", 1);
  buzzerCal(500,3);
  stepMotorlar.SMYKalibrasyon();
  buzzerOlumlu();
  bool butonKontrol = false;
  yaz("X EKSENI", 0);
  yaz("KALIBRASYON", 1);
  buzzerCal(500,3);
  stepMotorlar.SMXKalibrasyon();
  stepMotorlar.git(175,0);
  while(true){
    yaz("Anteni IHA'ya", 1);
    buzzerCal(500,3);
    yaz("cevirin.", 1);
    buzzerCal(500,3);
    yaz("Pusuladaki", 1);
    buzzerCal(500,3);
    yaz("kuzeyle olan aci", 1);
    buzzerCal(500,3);
    yaz("farkini PC'ye", 1);
    buzzerCal(500,3);
    yaz("girin ve", 1);
    buzzerCal(500,3);
    yaz("butona basin.", 1);
    buzzerCal(500,3);
    butonKontrol = butonDinle(5);
    if(butonKontrol){
      break;  
    }
  }
  delay(metinlerArasiBeklemeSuresi);
  yaz("KALIBRASYON", 0);
  yaz("TAMAMLANDI", 1);
  delay(metinlerArasiBeklemeSuresi);
  buzzerOlumlu();
  lcd.clear();
}
