const int stepPin = 2;
const int dirPin = 4;
int hiz;
int gidilecekAdimSayisi;
int a, b;
byte yonDegeri;

void setup() {
  yonDegeri = LOW;
  yonAyarla(yonDegeri); // Değişken türü hata çıkarabilir.
  hiz = 700;
  gidilecekAdimSayisi = 20;
  a = 20; // 100 veya 20
  b = 10;   // 2   veya 10
}

void loop() {
  git();
  yonAyarla(!yonDegeri);
  delay(1000);
}


void yonAyarla(byte yon){
  yonDegeri = yon;
  digitalWrite(dirPin, yonDegeri);
}

void birAdimAt(){
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(hiz);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(hiz);
}

void git(){
  for(int i = 0; i < a; i++){
    birAdimAt();
    hiz = hiz - b;
  }
  for(int i = 0; i < 5000; i++){
    birAdimAt();
  }
  for(int i = 0; i < a; i++){
    birAdimAt();
    hiz = hiz + b;
  }
}

