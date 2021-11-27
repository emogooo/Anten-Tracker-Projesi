#include "SM.h"

SM::SM(byte SMXStepPin, byte SMXDirPin, byte SMYStepPin, byte SMYDirPin, byte SMXLimit1Pin, byte SMXLimit2Pin, byte SMYLimit1Pin, byte SMYLimit2Pin){
  _SMXStepPin = SMXStepPin;
  _SMXDirPin = SMXDirPin;
  _SMYStepPin = SMYStepPin;
  _SMYDirPin = SMYDirPin;
  _SMXLimit1Pin = SMXLimit1Pin;
  _SMXLimit2Pin = SMXLimit2Pin;
  _SMYLimit1Pin = SMYLimit1Pin;
  _SMYLimit2Pin = SMYLimit2Pin;
  _baslangic();
}


void SM::_baslangic(){
  _xYonAyarla(HIGH);      // Değişken türü hata çıkarabilir.
  _yYonAyarla(HIGH);      // Değişken türü hata çıkarabilir.
  _xHiz = 500;
  _yHiz = 500;
  hizAyarla(500,400);
  _xEkseniAdimSayisi = 0;
  _yEkseniAdimSayisi = 0;
  _xAdimSay();
  _yAdimSay();
  _xKonumDerecesi = 360;
  _yKonumDerecesi = 225;
  git(180,112);
}

void SM::_xBirAdimAt(){
  digitalWrite(_SMXStepPin, HIGH);
  delayMicroseconds(_xHiz);
  digitalWrite(_SMXStepPin, LOW);
  delayMicroseconds(_xHiz);
}

void SM::_yBirAdimAt(){
  digitalWrite(_SMYStepPin, HIGH);
  delayMicroseconds(_yHiz);
  digitalWrite(_SMYStepPin, LOW);
  delayMicroseconds(_yHiz);
}

void SM::_xAdimSay(){
  while(digitalRead(_SMXLimit1Pin) == LOW){          // Yönden kaynaklı problem olabilir burası çok önemli !!!
    _xBirAdimAt();
  }
  _xYonAyarla(LOW);
  while(digitalRead(_SMXLimit2Pin) == LOW){
    _xBirAdimAt();
    _xEkseniAdimSayisi++;
  }
  _xYonAyarla(HIGH);
  for(byte i = 0; i < 5; i++){                     // Limit Butona baskı yapmaması için 5 adım geri döner. Burada ki 5 adım kör noktadır. Adım sayısı gerektiğinde yalnızca içeriden değiştirilebilir.
    _xBirAdimAt();
  }
  _xEkseni1DereceIcinAdimSayisi = (_xEkseniAdimSayisi - 10) / 360.0;
}

void SM::_yAdimSay(){
  while(digitalRead(_SMYLimit1Pin) == LOW){          // Yönden kaynaklı problem olabilir burası çok önemli !!!
    _yBirAdimAt();
  }
  _yYonAyarla(LOW);
  while(digitalRead(_SMYLimit2Pin) == LOW){
    _yBirAdimAt();
    _yEkseniAdimSayisi++;
  }
  _yYonAyarla(HIGH);
  for(byte i = 0; i < 5; i++){                     // Limit Butona baskı yapmaması için 5 adım geri döner. Burada ki 5 adım kör noktadır. Adım sayısı gerektiğinde yalnızca içeriden değiştirilebilir.
    _yBirAdimAt();
  }
  _yEkseni1DereceIcinAdimSayisi = (_yEkseniAdimSayisi - 10) / 225.0;
}

void SM::_xYonAyarla(byte yon){
  _SMXDir = yon;
  digitalWrite(_SMXDirPin, _SMXDir);
}

void SM::_yYonAyarla(byte yon){
  _SMYDir = yon;
  digitalWrite(_SMYDirPin, _SMYDir);
}

void SM::_xTekilHareket(int gidilecekKonumunDerecesi){
  // Yönün belirlenmesi.
  if(_xKonumDerecesi < gidilecekKonumunDerecesi){ // 0dan 100e
    _xYonAyarla(HIGH);
  }else{                                       // 100den 0a
    _xYonAyarla(LOW);
  }
  // Kaç derecelik açı yapacağının belirlenmesi.
  _xGidilecekDereceSayisi = abs(_xKonumDerecesi - gidilecekKonumunDerecesi);
  // Atılacak adımın belirlenmesi.
  _xGidilecekAdimSayisi = _xGidilecekDereceSayisi * _xEkseni1DereceIcinAdimSayisi;
  // Hızların belirlenmesi.
  if(_xGidilecekDereceSayisi < 5){  // Kısa mesafe hareket 1/32 için minimum 56 adım. 20-20 ivme adım payı.
    _xHareketIvmeAdimPayi = 20;
    _xHareketIvmeAdimBasinaDegisecekHiz = 10;
    _xHiz = _xHizTutucu + 300;
    _xGidilecekAdimSayisi = _xGidilecekAdimSayisi - 40;
  }else{  // Uzun mesafe hareket 1/32 için minimum 280 adım. 100-100 ivme adım payı.
    _xHareketIvmeAdimPayi = 100;
    _xHareketIvmeAdimBasinaDegisecekHiz = 2;
    _xHiz = _xHizTutucu;
    _xGidilecekAdimSayisi = _xGidilecekAdimSayisi - 200;
  }
  // Hareket fonksiyonu.
  for(byte i = 0; i < _xHareketIvmeAdimPayi; i++){
    _xBirAdimAt();
    _xHiz = _xHiz - _xHareketIvmeAdimBasinaDegisecekHiz;
  }
  for(byte i = 0; i < _xGidilecekAdimSayisi; i++){
    _xBirAdimAt();
  }
  for(byte i = 0; i < _xHareketIvmeAdimPayi; i++){
    _xBirAdimAt();
    _xHiz = _xHiz + _xHareketIvmeAdimBasinaDegisecekHiz;
  }  
  // Konum güncelleme.
  _xKonumDerecesi = gidilecekKonumunDerecesi;
}

void SM::_yTekilHareket(int gidilecekKonumunDerecesi){
  // Yönün belirlenmesi.
  if(_yKonumDerecesi < gidilecekKonumunDerecesi){ // 0dan 100e
    _yYonAyarla(HIGH);
  }else{                                       // 100den 0a
    _yYonAyarla(LOW);
  }
  // Kaç derecelik hareket yapacağının belirlenmesi.
  _yGidilecekDereceSayisi = abs(_yKonumDerecesi - gidilecekKonumunDerecesi);
  // Atılacak adımın belirlenmesi.
  _yGidilecekAdimSayisi = _yGidilecekDereceSayisi * _yEkseni1DereceIcinAdimSayisi;
  // Hızların belirlenmesi.
  if(_yGidilecekDereceSayisi < 5){  // Kısa mesafe hareket 1/32 için minimum 56 adım. 20-20 ivme adım payı.
    _yHareketIvmeAdimPayi = 20;
    _yHareketIvmeAdimBasinaDegisecekHiz = 10;
    _yHiz = _yHizTutucu + 300;
    _yGidilecekAdimSayisi = _yGidilecekAdimSayisi - 40;
  }else{  // Uzun mesafe hareket 1/32 için minimum 280 adım. 100-100 ivme adım payı.
    _yHareketIvmeAdimPayi = 100;
    _yHareketIvmeAdimBasinaDegisecekHiz = 2;
    _yHiz = _yHizTutucu;
    _yGidilecekAdimSayisi = _yGidilecekAdimSayisi - 200;
  }
  // Hareket fonksiyonu.
  for(byte i = 0; i < _yHareketIvmeAdimPayi; i++){
    _yBirAdimAt();
    _yHiz = _yHiz - _yHareketIvmeAdimBasinaDegisecekHiz;
  }
  for(byte i = 0; i < _yGidilecekAdimSayisi; i++){
    _yBirAdimAt();
  }
  for(byte i = 0; i < _yHareketIvmeAdimPayi; i++){
    _yBirAdimAt();
    _yHiz = _yHiz + _yHareketIvmeAdimBasinaDegisecekHiz;
  }
  // Konum güncelleme.
  _yKonumDerecesi = gidilecekKonumunDerecesi;
}

void SM::_cifteHareket(int xGidilecekKonumunDerecesi, int yGidilecekKonumunDerecesi){
  // Yönlerin belirlenmesi.
  if(_xKonumDerecesi < xGidilecekKonumunDerecesi){ // 0dan 100e
    _xYonAyarla(HIGH);
  }else{                                       // 100den 0a
    _xYonAyarla(LOW);
  }
  if(_yKonumDerecesi < yGidilecekKonumunDerecesi){ // 0dan 100e  
    _yYonAyarla(HIGH);
  }else{                                       // 100den 0a
    _yYonAyarla(LOW);
  }
  // Kaç derecelik hareket yapılacağının belirlenmesi.
  _xGidilecekDereceSayisi = abs(_xKonumDerecesi - xGidilecekKonumunDerecesi);
  _yGidilecekDereceSayisi = abs(_yKonumDerecesi - yGidilecekKonumunDerecesi);
  // Atılacak adımların belirlenmesi.
  _xGidilecekAdimSayisi = _xGidilecekDereceSayisi * _xEkseni1DereceIcinAdimSayisi;
  _yGidilecekAdimSayisi = _yGidilecekDereceSayisi * _yEkseni1DereceIcinAdimSayisi;
  //Hızların belirlenmesi.
  if(_xGidilecekDereceSayisi < 5){  // Kısa mesafe hareket 1/32 için minimum 56 adım. 20-20 ivme adım payı.
    _xHareketIvmeAdimPayi = 20;
    _xHareketIvmeAdimBasinaDegisecekHiz = 10;
    _xHiz = _xHizTutucu + 300;
  }else{  // Uzun mesafe hareket 1/32 için minimum 280 adım. 100-100 ivme adım payı.
    _xHareketIvmeAdimPayi = 100;
    _xHareketIvmeAdimBasinaDegisecekHiz = 2;
    _xHiz = _xHizTutucu;
  } 
  if(_yGidilecekDereceSayisi < 5){  // Kısa mesafe hareket 1/32 için minimum 56 adım. 20-20 ivme adım payı.
    _yHareketIvmeAdimPayi = 20;
    _yHareketIvmeAdimBasinaDegisecekHiz = 10;
    _yHiz = _yHizTutucu + 300;
  }else{  // Uzun mesafe hareket 1/32 için minimum 280 adım. 100-100 ivme adım payı.
    _yHareketIvmeAdimPayi = 100;
    _yHareketIvmeAdimBasinaDegisecekHiz = 2;
    _yHiz = _yHizTutucu;
  }
  // Hareket fonksiyonu.
  _xGidilenAdimSayisi = 0;
  _yGidilenAdimSayisi = 0;
  _xGidilecekNihaiAdimSayisi = _xGidilecekAdimSayisi;
  _yGidilecekNihaiAdimSayisi = _yGidilecekAdimSayisi;
  while(_xGidilecekAdimSayisi > 0 || _yGidilecekAdimSayisi > 0){
    if(_xGidilecekAdimSayisi > 0){
      if(_xGidilenAdimSayisi < _xHareketIvmeAdimPayi){
        _xHiz = _xHiz - _xHareketIvmeAdimBasinaDegisecekHiz;
      }else if(_xGidilecekNihaiAdimSayisi - _xGidilenAdimSayisi < _xHareketIvmeAdimPayi){
        _xHiz = _xHiz + _xHareketIvmeAdimBasinaDegisecekHiz;
      }
      _xBirAdimAt();
      _xGidilecekAdimSayisi--;
      _xGidilenAdimSayisi++;
    }
    if(_yGidilecekAdimSayisi > 0){
      if(_yGidilenAdimSayisi < _yHareketIvmeAdimPayi){
        _yHiz = _yHiz - _yHareketIvmeAdimBasinaDegisecekHiz;
      }else if(_yGidilecekNihaiAdimSayisi - _yGidilenAdimSayisi < _yHareketIvmeAdimPayi){
        _yHiz = _yHiz + _yHareketIvmeAdimBasinaDegisecekHiz;
      }
      _yBirAdimAt();
      _yGidilecekAdimSayisi--;
      _yGidilenAdimSayisi++;
    }
  }
  // Konum güncelleme.
  _xKonumDerecesi = xGidilecekKonumunDerecesi;
  _yKonumDerecesi = yGidilecekKonumunDerecesi;
}

void SM::hizAyarla(int xHiz, int yHiz){
  _xHizTutucu = xHiz;
  _yHizTutucu = yHiz;
}

void SM::git(int xGidilecekKonumunDerecesi, int yGidilecekKonumunDerecesi){
  if(xGidilecekKonumunDerecesi != _xKonumDerecesi && yGidilecekKonumunDerecesi != _yKonumDerecesi){
    _cifteHareket(xGidilecekKonumunDerecesi, yGidilecekKonumunDerecesi);
  }else if(xGidilecekKonumunDerecesi != _xKonumDerecesi){
    _xTekilHareket(xGidilecekKonumunDerecesi);
  }else{
    _yTekilHareket(yGidilecekKonumunDerecesi);
  }
}
