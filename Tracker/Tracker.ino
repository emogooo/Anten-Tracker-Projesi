#include "SM.h"
#include "rgb_lcd.h"
#include <SoftwareSerial.h>

#define parkButtonPin A3
#define batteryVoltagePin A2
#define input5VoltagePin A1
#define input12VoltagePin A0
#define calibrationButtonPin 2
#define interruptPinValue 1 // Kartta 3. pin, fakat sistem bunu 1 olarak okuyor.
#define buzzerPin 4
#define SMXHighDegreeLimitPin 10
#define SMXLowerDegreeLimitPin 9
#define SMYHighDegreeLimitPin 11
#define SMYLowerDegreeLimitPin 12
#define SMXDirectionPin 6
#define SMXStepPin 5
#define SMYDirectionPin 8
#define SMYStepPin 7

rgb_lcd lcd;
SM stepperMotors(SMXStepPin, SMXDirectionPin, SMYStepPin, SMYDirectionPin, SMXHighDegreeLimitPin, SMXLowerDegreeLimitPin, SMYHighDegreeLimitPin, SMYLowerDegreeLimitPin);

String rawData;
int degreeToGoMotorX = 0;
int degreeToGoMotorY = 0;
unsigned long lastWorkingTimeLimitX = 0;
unsigned long lastWorkingTimeLimitY = 0;
byte batteryLevel[8];
float voltage;
float lastVoltage;
bool emergencyBuzzerControl;

void setup() {
  batteryLevel[0] = B01110;
  batteryLevel[7] = B11111;
  lastVoltage = -1;
  emergencyBuzzerControl = false;
  pinMode(calibrationButtonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  lcd.begin(16, 2);
  lcd.clear();  
  startSystem();
  stepperMotors.xHizAyarla(250,90);
  stepperMotors.yHizAyarla(500,150);
  attachInterrupt(interruptPinValue, interruptFunction, RISING);
  Serial.begin(9600);
  printLCDMiddle("  Y          X  ", 0);
  printLCDMiddle(" 0         155", 1);
}

void loop() {
  voltage = analogRead(batteryVoltagePin) * 0.0278662;
  if (abs(voltage - lastVoltage) > 0.2) {
    printBatteryLevel(voltage);
    lastVoltage = voltage;
    if (voltage <= 20) {
      emergencyBuzzerControl = true;
    } else {
      emergencyBuzzerControl = false;
    }
  }

  if(emergencyBuzzerControl){
    playBuzzer(250, 1);
  }

  if(digitalRead(parkButtonPin) == HIGH){
    stepperMotors.git(151, 0);
    printLCDMiddle(" 0         151", 1);
  }

  if(Serial.available()){
    rawData = Serial.readString();
    if(proceedReceivedData()){
      Serial.println("*0#");
      printLCDMiddle((String(degreeToGoMotorY) + "          " + String(degreeToGoMotorX)), 1);     
      stepperMotors.git(degreeToGoMotorX, degreeToGoMotorY);
      Serial.println("*1#");
    }
  }
}

void interruptFunction(){
  if(digitalRead(SMXHighDegreeLimitPin) == HIGH && millis() - lastWorkingTimeLimitX > 1000 ){
    stepperMotors.yonDegistir(true);
    lastWorkingTimeLimitX = millis();
    playBuzzer(250,3);
  }else if(digitalRead(SMXLowerDegreeLimitPin) == HIGH && millis() - lastWorkingTimeLimitX > 1000 ){
    stepperMotors.yonDegistir(true);
    lastWorkingTimeLimitX = millis();
    playBuzzer(250,3);
  }else if(digitalRead(SMYHighDegreeLimitPin) == HIGH && millis() - lastWorkingTimeLimitY > 1000 ){
    stepperMotors.yonDegistir(false);
    lastWorkingTimeLimitY = millis();
    playBuzzer(250,3);
  }else if(digitalRead(SMYLowerDegreeLimitPin) == HIGH && millis() - lastWorkingTimeLimitY > 1000 ){
    stepperMotors.yonDegistir(false);
    lastWorkingTimeLimitY = millis();
    playBuzzer(250,3);
  }
}

bool proceedReceivedData(){
  int startIndex = rawData.indexOf('#');
  if(startIndex == -1){
    return false;
  }
  int  endIndex = rawData.indexOf('*', startIndex);
  if(endIndex == -1 || endIndex - startIndex > 8){
    return false;
  }
  rawData = rawData.substring(startIndex + 1, endIndex);
  int dotIndex = rawData.indexOf('.');
  if(dotIndex == -1){
    return false;
  }
  degreeToGoMotorX = (rawData.substring(0,dotIndex)).toInt();
  degreeToGoMotorY = (rawData.substring((dotIndex + 1), (rawData.length()))).toInt();
  return true;
}

void printLCDMiddle(String text, byte row){
  for(int i = 0; i < 16; i++){
    lcd.setCursor(i, (row % 2));
    lcd.print(" ");
  }
  lcd.setCursor(((16 - text.length()) / 2), (row % 2));
  lcd.print(text);
}

bool listenCalibrationButton(unsigned int second){
  unsigned long baslangic = millis();
  while(millis() - baslangic <= second * 1000){
        if(digitalRead(calibrationButtonPin)){
          playPositiveSound();
          return true;
        }
  }
  return false;
}

void playBuzzer(unsigned int ms, unsigned int count){
  for(unsigned int i = 0; i < count; i++){
    digitalWrite(buzzerPin, HIGH);
    delay(ms);
    digitalWrite(buzzerPin, LOW);  
    delay(ms);
  }
}

void playPositiveSound(){
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

void startSystem(){
  int waitingTime = 3000;
  printLCDMiddle("BMS SAVUNMA", 0);
  printLCDMiddle("TEKNOLOJILERI", 1);
  delay(waitingTime);
  printLCDMiddle("ANTEN TRACKER", 0);
  printLCDMiddle("VERSIYON 1.0", 1);
  delay(waitingTime);
  printLCDMiddle("BMS SAVUNMA TEK.", 0);
  printLCDMiddle("V1      V2     ", 1);
  float voltage5 = analogRead(input5VoltagePin);
  voltage5 = voltage5 * 0.007088068; 
  float voltage12 = analogRead(input12VoltagePin);
  voltage12 = (voltage12 * 0.015625) + 0.7; // 0.7 diyot kırılma voltajı. 
  lcd.setCursor(3, 1);
  lcd.print(voltage5, 2);
  lcd.setCursor(11, 1);
  lcd.print(voltage12, 2);
  delay(waitingTime);
  printLCDMiddle("BATARYA V       ", 1);
  float voltageBattery = analogRead(batteryVoltagePin);
  voltageBattery = voltageBattery * 0.0278662;
  lcd.setCursor(11, 1);
  lcd.print(voltageBattery, 2);
  delay(waitingTime);
  if((voltage12 > 10.5 && voltage12 < 14) && (voltage5 > 4.5 && voltage5 < 5.3) && voltageBattery > 20 ){
      printLCDMiddle("BMS SAVUNMA TEK.", 0);
      printLCDMiddle("VOLTAJLAR UYGUN", 1);
      delay(waitingTime);
      playPositiveSound();
  }else{
      printLCDMiddle("BMS SAVUNMA TEK.", 0);
      printLCDMiddle("VOLTAJ ARIZASI", 1);
      while(true){
        playBuzzer(250, 1);
      }      
  }
  printLCDMiddle("ANTEN TRACKER", 0);
  printLCDMiddle("KALIBRASYON", 1);
  playBuzzer(500,3);
  printLCDMiddle("Y EKSENI", 0);
  printLCDMiddle("KALIBRASYON", 1);
  playBuzzer(500,3);
  stepperMotors.SMYKalibrasyon();
  playPositiveSound();
  bool buttonCalibrationControl = false;
  printLCDMiddle("X EKSENI", 0);
  printLCDMiddle("KALIBRASYON", 1);
  playBuzzer(500,3);
  stepperMotors.SMXKalibrasyon();
  stepperMotors.xHizAyarla(30,60);
  stepperMotors.git(155,0);
  while(true){
    printLCDMiddle("Sehpayi IHA'ya", 1);
    playBuzzer(500,3);
    printLCDMiddle("cevirin.", 1);
    playBuzzer(500,3);
    printLCDMiddle("Pusuladaki", 1);
    playBuzzer(500,3);
    printLCDMiddle("kuzeyle olan aci", 1);
    playBuzzer(500,3);
    printLCDMiddle("farkini PC'ye", 1);
    playBuzzer(500,3);
    printLCDMiddle("girin ve", 1);
    playBuzzer(500,3);
    printLCDMiddle("butona basin.", 1);
    playBuzzer(500,3);
    buttonCalibrationControl = listenCalibrationButton(5);
    if(buttonCalibrationControl){
      break;  
    }
  }
  delay(waitingTime);
  printLCDMiddle("KALIBRASYON", 0);
  printLCDMiddle("TAMAMLANDI", 1);
  delay(waitingTime);
  playPositiveSound();
  lcd.clear();
}

void printBatteryLevel(float voltage) {
  if (voltage >= 23.5) {
    batteryLevel[1] = B11111;
    batteryLevel[2] = B11111;
    batteryLevel[3] = B11111;
    batteryLevel[4] = B11111;
    batteryLevel[5] = B11111;
    batteryLevel[6] = B11111;
  }
  if (voltage < 23.5 && voltage > 20) {
    batteryLevel[1] = B10001;
    batteryLevel[2] = B10001;
    batteryLevel[3] = B10001;
    batteryLevel[4] = B11111;
    batteryLevel[5] = B11111;
    batteryLevel[6] = B11111;
  }
  if (voltage <= 20) {
    batteryLevel[1] = B10001;
    batteryLevel[2] = B10001;
    batteryLevel[3] = B10001;
    batteryLevel[4] = B10001;
    batteryLevel[5] = B10001;
    batteryLevel[6] = B10001;
  }
  lcd.createChar(0, batteryLevel);
  lcd.setCursor(7, 0);
  lcd.write(byte(0));
}
