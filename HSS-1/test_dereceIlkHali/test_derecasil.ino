#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 6, 5, 4, 3); 
int degerx=0;
int degery=0;
#define Buton 8

void setup() 
{
  lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(Buton, INPUT);
//  Serial.println("Pot Acı Okuma");
}
void loop() 
{
  degerx  = analogRead(A1);
int xderece = 0.35156*degerx; 
  
  degery  = analogRead(A0);
int yderece = 0.26367*degery; 
 
  lcd.setCursor(0, 0);
  lcd.print(xderece);
  lcd.setCursor(5, 0);
  lcd.print("derece");
  lcd.setCursor(13, 0);
  lcd.print(yderece);
  delay(500);
  lcd.clear();

  if (digitalRead(Buton) == 1){
  Serial.print(xderece); 
  Serial.print(".");
  Serial.println(yderece); 
  lcd.setCursor(3, 1);
  lcd.print(" Transmit ");
   }
  else  
  delay(10);
  
}
