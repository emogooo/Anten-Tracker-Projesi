int speakerPin = A3; // buzzer bağlantı pini
int length = 15; // toplam nota sayısı
char notes[] = "ccggaagffeeddc "; // sondaki boşluk es vermek için
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;
 
void playTone(int tone, int duration) {
for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);  }
}
 
// TimeHigh değerini belirli notlara ayarlama
void playNote(char note, int duration) {
char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
 
for (int i = 0; i < 8; i++) { // Notaya karşılık gelen ses tonu
    if (names[i] == note) {
        playTone(tones[i], duration);   }
   }
}
 
void setup() {
pinMode(speakerPin, OUTPUT);
}
// melodiyi çalalım
void loop() {

}
