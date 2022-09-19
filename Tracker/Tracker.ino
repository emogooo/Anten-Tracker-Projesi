#include "SM.h"
#include "rgb_lcd.h"  // Grove - LCD RGB Blacklight 1.0.0 Library
#include <SoftwareSerial.h>
#define giris5V A1
#define giris12V A0
#define kalibrasyonButonPin 2
#define buzzerPin 4
#define parkButonPin A3
#define SMXEndLimitPin 10
#define SMXStartLimitPin 9
#define SMYEndLimitPin 11
#define SMYStartLimitPin 12
#define SMXDirPin 6
#define SMXStepPin 5
#define SMYDirPin 8
#define SMYStepPin 7
#define kesmePinDegeri 1  // Kartta 3. pin, fakat sistem bunu 1 olarak okuyor.
const unsigned int xEkseniDerecesi = 310;
const unsigned int yEkseniDerecesi = 125;
String hamVeri = "";
char veriBirimi;
int gidilecekDereceMotorX = 0;
int gidilecekDereceMotorY = 0;
unsigned long xLimitSonCalismaZamani = 0;
unsigned long yLimitSonCalismaZamani = 0;
rgb_lcd lcd;
SM stepMotorlar(SMXStepPin, SMXDirPin, SMYStepPin, SMYDirPin, SMXEndLimitPin, SMXStartLimitPin, SMYEndLimitPin, SMYStartLimitPin, xEkseniDerecesi, yEkseniDerecesi);

void setup() {
  pinMode(kalibrasyonButonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  lcd.begin(16, 2);
  lcd.clear();
  kalibrasyon();
  stepMotorlar.xHizAyarla(30, 60);
  stepMotorlar.yHizAyarla(30, 60);
  attachInterrupt(kesmePinDegeri, limitKesmeFonksiyonu, RISING);
  Serial.begin(9600);
  yaz("  Y          X  ", 0);
}

void loop() {
  if (digitalRead(parkButonPin) == HIGH) {
    stepMotorlar.git(151, 0);
  }
  while (Serial.available()) {
    veriBirimi = (char)Serial.read();
    hamVeri += veriBirimi;
    if (veriBirimi == '\n') {
      veriIsle();
      yaz((String(gidilecekDereceMotorY) + "          " + String(gidilecekDereceMotorX)), 1);
      stepMotorlar.git(gidilecekDereceMotorX, gidilecekDereceMotorY);
    }
  }
}

void limitKesmeFonksiyonu() {
  if (digitalRead(SMXEndLimitPin) == HIGH && millis() - xLimitSonCalismaZamani > 1000) {
    stepMotorlar.yonDegistir(true);
    xLimitSonCalismaZamani = millis();
  } else if (digitalRead(SMXStartLimitPin) == HIGH && millis() - xLimitSonCalismaZamani > 1000) {
    stepMotorlar.yonDegistir(true);
    xLimitSonCalismaZamani = millis();
  } else if (digitalRead(SMYEndLimitPin) == HIGH && millis() - yLimitSonCalismaZamani > 1000) {
    stepMotorlar.yonDegistir(false);
    yLimitSonCalismaZamani = millis();
  } else if (digitalRead(SMYStartLimitPin) == HIGH && millis() - yLimitSonCalismaZamani > 1000) {
    stepMotorlar.yonDegistir(false);
    yLimitSonCalismaZamani = millis();
  }
}

void veriIsle() {
  int noktaIndisi = 0;
  noktaIndisi = hamVeri.indexOf('.', 1);
  gidilecekDereceMotorX = (hamVeri.substring(0, noktaIndisi)).toInt();
  gidilecekDereceMotorY = (hamVeri.substring((noktaIndisi + 1), (hamVeri.length() - 2))).toInt();
  hamVeri = "";
}

void yaz(String yazi, byte satir) {
  for (int i = 0; i < 16; i++) {
    lcd.setCursor(i, (satir % 2));
    lcd.print(" ");
  }
  lcd.setCursor(((16 - yazi.length()) / 2), (satir % 2));
  lcd.print(yazi);
}

bool kalibrasyonButonuDinle(unsigned int saniye) {
  unsigned long baslangic = millis();
  while (millis() - baslangic <= saniye * 1000) {
    if (digitalRead(kalibrasyonButonPin)) {
      buzzerOlumlu();
      return true;
    }
  }
  return false;
}

void buzzerCal(unsigned int ms, unsigned int adet) {
  for (unsigned int i = 0; i < adet; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(ms);
    digitalWrite(buzzerPin, LOW);
    delay(ms);
  }
}

void buzzerOlumlu() {
  digitalWrite(buzzerPin, HIGH);
  delay(500);
  digitalWrite(buzzerPin, LOW);
  delay(100);
  digitalWrite(buzzerPin, HIGH);
  delay(100);
  digitalWrite(buzzerPin, LOW);
  delay(100);
  digitalWrite(buzzerPin, HIGH);
  delay(100);
  digitalWrite(buzzerPin, LOW);
  delay(1000);
}

void kalibrasyon() {
  int metinlerArasiBeklemeSuresi = 3000;
  yaz("BMS SAVUNMA", 0);
  yaz("TEKNOLOJILERI", 1);
  delay(metinlerArasiBeklemeSuresi);
  yaz("ANTEN TRACKER", 0);
  yaz("VERSIYON 1.0", 1);
  delay(metinlerArasiBeklemeSuresi);
  yaz("BMS SAVUNMA TEK.", 0);
  yaz("V1      V2     ", 1);

  float voltaj5 = analogRead(giris5V);
  voltaj5 = voltaj5 * 0.007088068;
  float voltaj12 = analogRead(giris12V);
  voltaj12 = voltaj12 * 0.01762247;

  lcd.setCursor(3, 1);
  lcd.print(voltaj5, 2);
  lcd.setCursor(11, 1);
  lcd.print(voltaj12, 2);
  delay(metinlerArasiBeklemeSuresi);
  if (true) {  //(voltaj12 > 10.5 && voltaj12 < 14) && (voltaj5 > 4.5 && voltaj5 < 5.3)
    yaz("BMS SAVUNMA TEK.", 0);
    yaz("VOLTAJLAR UYGUN", 1);
    delay(metinlerArasiBeklemeSuresi);
    buzzerOlumlu();
  } else {
    yaz("BMS SAVUNMA TEK.", 0);
    yaz("VOLTAJ ARIZASI", 1);
    while (true) {
      buzzerCal(250, 1);
    }
  }
  yaz("ANTEN TRAKER", 0);
  yaz("KALIBRASYON", 1);
  buzzerCal(500, 3);
  yaz("Y EKSENI", 0);
  yaz("KALIBRASYON", 1);
  buzzerCal(500, 3);
  stepMotorlar.SMYKalibrasyon();
  buzzerOlumlu();
  bool butonKontrol = false;
  yaz("X EKSENI", 0);
  yaz("KALIBRASYON", 1);
  buzzerCal(500, 3);

  stepMotorlar.SMXKalibrasyon();
  stepMotorlar.git((xEkseniDerecesi / 2), 0);

  while (true) {
    yaz("Sehpayi IHA'ya", 1);
    buzzerCal(500, 3);
    yaz("cevirin.", 1);
    buzzerCal(500, 3);
    yaz("Pusuladaki", 1);
    buzzerCal(500, 3);
    yaz("kuzeyle olan aci", 1);
    buzzerCal(500, 3);
    yaz("farkini PC'ye", 1);
    buzzerCal(500, 3);
    yaz("girin ve", 1);
    buzzerCal(500, 3);
    yaz("butona basin.", 1);
    buzzerCal(500, 3);
    butonKontrol = kalibrasyonButonuDinle(5);
    if (butonKontrol) {
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