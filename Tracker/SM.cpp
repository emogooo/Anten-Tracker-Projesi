#include "SM.h"

SM::SM(byte SMXStepPin, byte SMXDirPin, byte SMYStepPin, byte SMYDirPin, byte SMX350DereceLimitPin, byte SMX0DereceLimitPin, byte SMY90DereceLimitPin, byte SMY0DereceLimitPin){
  _SMXStepPin = SMXStepPin;
  _SMXDirPin = SMXDirPin;
  _SMYStepPin = SMYStepPin;
  _SMYDirPin = SMYDirPin;
  _SMX350DereceLimitPin = SMX350DereceLimitPin;
  _SMX0DereceLimitPin = SMX0DereceLimitPin;
  _SMY90DereceLimitPin = SMY90DereceLimitPin;
  _SMY0DereceLimitPin = SMY0DereceLimitPin;
  xHizAyarla(50,100);
  yHizAyarla(50,100);
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

void SM::_xCifteHareketBirAdimAt(){
  digitalWrite(_SMXStepPin, HIGH);
  delayMicroseconds(_xHiz);
  digitalWrite(_SMXStepPin, LOW);
}

void SM::_yCifteHareketBirAdimAt(){
  digitalWrite(_SMYStepPin, HIGH);
  delayMicroseconds(_yHiz);
  digitalWrite(_SMYStepPin, LOW);
}

unsigned long SM::SMXKalibrasyon(){
  _xYonAyarla(HIGH);
  _xHiz = 30; // Önceki motor değer 400
  _xEkseniAdimSayisi = 0;
  while(digitalRead(_SMX350DereceLimitPin) == LOW){          // Yönden kaynaklı problem olabilir burası çok önemli !!!
    _xBirAdimAt();
  }
  _xYonAyarla(!_SMXDir);
  while(digitalRead(_SMX0DereceLimitPin) == LOW){
    _xBirAdimAt();
    _xEkseniAdimSayisi++;
  }
  _xYonAyarla(!_SMXDir);
  for(int i = 0; i < 500; i++){                     // Limit Butona baskı yapmaması için 5 adım geri döner. Burada ki 5 adım kör noktadır. Adım sayısı gerektiğinde yalnızca içeriden değiştirilebilir.
    _xBirAdimAt();
  }
  _xEkseni1DereceIcinAdimSayisi = (_xEkseniAdimSayisi - 1000) / 310;
  _xKonumDerecesi = 0;
  return _xEkseniAdimSayisi;
}
unsigned long SM::SMYKalibrasyon(){
  _yYonAyarla(HIGH);
  _yHiz = 30; // Önceki motor değeri 200
  _yEkseniAdimSayisi = 0;
  while(digitalRead(_SMY90DereceLimitPin) == LOW){          // Yönden kaynaklı problem olabilir burası çok önemli !!! Yukarı hareket ediyor
    _yBirAdimAt();
  }
  _yYonAyarla(!_SMYDir);
  while(digitalRead(_SMY0DereceLimitPin) == LOW){         // Aşağı hareket ediyor 
    _yBirAdimAt();
    _yEkseniAdimSayisi++;
  }
  _yYonAyarla(!_SMYDir);
  for(int i = 0; i < 1000; i++){                     // Limit Butona baskı yapmaması için 5 adım geri döner. Burada ki 5 adım kör noktadır. Adım sayısı gerektiğinde yalnızca içeriden değiştirilebilir.
    _yBirAdimAt();
  }
  _yEkseni1DereceIcinAdimSayisi = (_yEkseniAdimSayisi - 2000) / 125;
  _yKonumDerecesi = 0;
  return _yEkseniAdimSayisi;
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
  // Kaç derecelik hareket yapacağının belirlenmesi.
  _xGidilecekDereceSayisi = abs(_xKonumDerecesi - gidilecekKonumunDerecesi);
  // Atılacak adımın belirlenmesi.
  _xGidilecekAdimSayisi = _xGidilecekDereceSayisi * _xEkseni1DereceIcinAdimSayisi;
  // Hızların belirlenmesi.
  if(_xGidilecekAdimSayisi < 1200){  // Kısa mesafe hareket 1/32 için minimum 1 derecelik adım sayısına göre ivmelenir.
    _xHareketIvmeAdimPayi = _xEkseni1DereceIcinAdimSayisi / 2;
    _xHareketIvmeAdimBasinaDegisecekHiz = 10;
    _xHiz = _xYavasHiz + (_xHareketIvmeAdimPayi * _xHareketIvmeAdimBasinaDegisecekHiz); 
  }else{  // Uzun mesafe hareket 1/32 için minimum 1200 adım. 600-600 ivme adım payı.
    _xHareketIvmeAdimPayi = 600;
    _xHareketIvmeAdimBasinaDegisecekHiz = 1;
    _xHiz = _xHizliHiz + (_xHareketIvmeAdimPayi * _xHareketIvmeAdimBasinaDegisecekHiz);
  }
  _xGidilecekAdimSayisi = _xGidilecekAdimSayisi - (_xHareketIvmeAdimPayi * 2);
  // Hareket fonksiyonu.
  for(int i = 0; i < _xHareketIvmeAdimPayi; i++){
    _xBirAdimAt();
    _xHiz = _xHiz - _xHareketIvmeAdimBasinaDegisecekHiz;
  }
  for(unsigned long i = 0; i < _xGidilecekAdimSayisi; i++){
    _xBirAdimAt();
  }
  for(int i = 0; i < _xHareketIvmeAdimPayi; i++){
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
  if(_yGidilecekAdimSayisi < 1200){  // Kısa mesafe hareket 1/32 için minimum 1 derecelik adım sayısına göre ivmelenir.
    _yHareketIvmeAdimPayi = _yEkseni1DereceIcinAdimSayisi / 2;
    _yHareketIvmeAdimBasinaDegisecekHiz = 10;
    _yHiz = _yYavasHiz + (_yHareketIvmeAdimPayi * _yHareketIvmeAdimBasinaDegisecekHiz); 
  }else{  // Uzun mesafe hareket 1/32 için minimum 1200 adım. 600-600 ivme adım payı.
    _yHareketIvmeAdimPayi = 600;
    _yHareketIvmeAdimBasinaDegisecekHiz = 1;
    _yHiz = _yHizliHiz + (_yHareketIvmeAdimPayi * _yHareketIvmeAdimBasinaDegisecekHiz); 
  }
  _yGidilecekAdimSayisi = _yGidilecekAdimSayisi - (_yHareketIvmeAdimPayi * 2);
  // Hareket fonksiyonu.
  for(int i = 0; i < _yHareketIvmeAdimPayi; i++){
    _yBirAdimAt();
    _yHiz = _yHiz - _yHareketIvmeAdimBasinaDegisecekHiz;
  }
  for(unsigned long i = 0; i < _yGidilecekAdimSayisi; i++){
    _yBirAdimAt();
  }
  for(int i = 0; i < _yHareketIvmeAdimPayi; i++){
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
  if(_xGidilecekAdimSayisi < 1200){  // Kısa mesafe hareket 1/32 için minimum 1 derecelik adım sayısına göre ivmelenir.
    _xHareketIvmeAdimPayi = _xEkseni1DereceIcinAdimSayisi / 2;
    _xHareketIvmeAdimBasinaDegisecekHiz = 10;
    _xHiz = _xYavasHiz + (_xHareketIvmeAdimPayi * _xHareketIvmeAdimBasinaDegisecekHiz); 
  }else{  // Uzun mesafe hareket 1/32 için minimum 1200 adım. 600-600 ivme adım payı.
    _xHareketIvmeAdimPayi = 600;
    _xHareketIvmeAdimBasinaDegisecekHiz = 1;
    _xHiz = _xHizliHiz + (_xHareketIvmeAdimPayi * _xHareketIvmeAdimBasinaDegisecekHiz);
  }
  if(_yGidilecekAdimSayisi < 1200){  // Kısa mesafe hareket 1/32 için minimum 1 derecelik adım sayısına göre ivmelenir.
    _yHareketIvmeAdimPayi = _yEkseni1DereceIcinAdimSayisi / 2;
    _yHareketIvmeAdimBasinaDegisecekHiz = 10;
    _yHiz = _yYavasHiz + (_yHareketIvmeAdimPayi * _yHareketIvmeAdimBasinaDegisecekHiz); 
  }else{  // Uzun mesafe hareket 1/32 için minimum 1200 adım. 600-600 ivme adım payı.
    _yHareketIvmeAdimPayi = 600;
    _yHareketIvmeAdimBasinaDegisecekHiz = 1;
    _yHiz = _yHizliHiz + (_yHareketIvmeAdimPayi * _yHareketIvmeAdimBasinaDegisecekHiz); 
  }
  // Hareket fonksiyonu.
  _xGidilenAdimSayisi = 0;
  _yGidilenAdimSayisi = 0;
  while((_xGidilecekAdimSayisi - _xGidilenAdimSayisi) > 0 || (_yGidilecekAdimSayisi - _yGidilenAdimSayisi) > 0){
    if((_xGidilecekAdimSayisi - _xGidilenAdimSayisi) > 0){
      if(_xGidilenAdimSayisi < _xHareketIvmeAdimPayi){
        _xHiz = _xHiz - _xHareketIvmeAdimBasinaDegisecekHiz;
      }else if(_xGidilecekAdimSayisi - _xGidilenAdimSayisi < _xHareketIvmeAdimPayi){
        _xHiz = _xHiz + _xHareketIvmeAdimBasinaDegisecekHiz;
      }
      _xCifteHareketBirAdimAt();
      _xGidilenAdimSayisi++;
    }else{
      if(_xHiz > _yHiz){
        _xHiz = _xHiz - _xHareketIvmeAdimBasinaDegisecekHiz;
      }
      delayMicroseconds(_xHiz);
    }
    if((_yGidilecekAdimSayisi - _yGidilenAdimSayisi) > 0){
      if(_yGidilenAdimSayisi < _yHareketIvmeAdimPayi){
        _yHiz = _yHiz - _yHareketIvmeAdimBasinaDegisecekHiz;
      }else if((_yGidilecekAdimSayisi - _yGidilenAdimSayisi) < _yHareketIvmeAdimPayi){
        _yHiz = _yHiz + _yHareketIvmeAdimBasinaDegisecekHiz;
      }
      _yCifteHareketBirAdimAt();
      _yGidilenAdimSayisi++;
    }else{
      if(_yHiz > _xHiz){
        _yHiz = _yHiz - _yHareketIvmeAdimBasinaDegisecekHiz;
      }
      delayMicroseconds(_yHiz);
    }
  }
  // Konum güncelleme.
  _xKonumDerecesi = xGidilecekKonumunDerecesi;
  _yKonumDerecesi = yGidilecekKonumunDerecesi;
}

void SM::yonDegistir(bool kontrol){
  if(kontrol){
    _xYonAyarla(!_SMXDir);
  }else{
    _yYonAyarla(!_SMYDir);
  } 
}

void SM::xHizAyarla(int yavasHiz, int hizliHiz){
  _xYavasHiz = yavasHiz;
  _xHizliHiz = hizliHiz;
}

void SM::yHizAyarla(int yavasHiz, int hizliHiz){
  _yYavasHiz = yavasHiz;
  _yHizliHiz = hizliHiz;
}

void SM::git(int xGidilecekKonumunDerecesi, int yGidilecekKonumunDerecesi){
  if(xGidilecekKonumunDerecesi > 309 || xGidilecekKonumunDerecesi < 0){
    return;
  }
  if(yGidilecekKonumunDerecesi > 124 || yGidilecekKonumunDerecesi < 0){
    return;
  }
  if(xGidilecekKonumunDerecesi != _xKonumDerecesi && yGidilecekKonumunDerecesi != _yKonumDerecesi){
    _cifteHareket(xGidilecekKonumunDerecesi, yGidilecekKonumunDerecesi);
  }else if(xGidilecekKonumunDerecesi != _xKonumDerecesi){
    _xTekilHareket(xGidilecekKonumunDerecesi);
  }else if(yGidilecekKonumunDerecesi != _yKonumDerecesi){
    _yTekilHareket(yGidilecekKonumunDerecesi);
  }
}
