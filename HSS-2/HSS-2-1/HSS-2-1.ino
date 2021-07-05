#include "rgb_lcd.h"
rgb_lcd lcd;

void setup() {
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
  yaz("VOLT1   VOLT2", 1);
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
  yaz("Teraziyi duz olmasi icin ayarlayin. Duz olunca butona basin.", 1);
  for(int i = 0; i < 4; i++){ // Burada buton beklenecek
    buzzerCal();
  }
  delay(4000);
  lcd.clear();
  yaz("Y EKSENI", 0);
  yaz("Y Ekseni motoru 0 dereceye gelip durdugunda zemin ile paralel ise butona basin.", 1);
  for(int i = 0; i < 4; i++){ // Burada buton beklenecek
    buzzerCal();
  }
  delay(4000);
  lcd.clear();
  yaz("X EKSENI", 0);
  yaz("Pusulayi yerlestirin. X Ekseni motoru 0 dereceye gelip durdugunda 0 dereceyi ucus planinizin en uzak kismina getirmek icin sehpayi donderin. Teraziyi kontrol edin. Pusuladaki kuzeyle olan aci farkini PC'ye girin butona basin.", 1);
  for(int i = 0; i < 4; i++){ // Burada buton beklenecek
    buzzerCal();
  }
  delay(4000);
  lcd.clear();
  yaz("KALIBRASYON", 0);
  yaz("TAMAMLANDI", 1);
  delay(4000);
}

void loop() {

}
// 16 harften kısa olan yazıları ortalayıp; uzunları ise sola kaydırarak görüntüler.
void yaz(String yazi, byte satir){
  if(yazi.length() <= 16){
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

void buzzerCal(){
  /*digitalWrite(x, HIGH);
  delay(500);
  digitalWrite(x, LOW);  
  delay(500);*/
}
