#include "Arduino.h"

class SM{
  private:
  byte _SMXStepPin;
  byte _SMXDirPin;
  byte _SMYStepPin;
  byte _SMYDirPin;
  byte _SMX350DereceLimitPin;
  byte _SMX0DereceLimitPin;
  byte _SMY90DereceLimitPin;
  byte _SMY0DereceLimitPin;
  byte _SMXDir;
  byte _SMYDir;
  int _xHiz;
  int _yHiz;
  int _xHizTutucu;
  int _yHizTutucu;
  unsigned long _xEkseniAdimSayisi;
  unsigned long _yEkseniAdimSayisi;
  int _xKonumDerecesi;
  int _yKonumDerecesi;
  int _xGidilecekDereceSayisi;
  int _yGidilecekDereceSayisi;
  unsigned long _xGidilecekAdimSayisi;
  unsigned long _yGidilecekAdimSayisi;
  unsigned long _xGidilenAdimSayisi;
  unsigned long _yGidilenAdimSayisi;
  int _xHareketIvmeAdimPayi;
  int _yHareketIvmeAdimPayi;
  int _xHareketIvmeAdimBasinaDegisecekHiz;
  int _yHareketIvmeAdimBasinaDegisecekHiz;
  unsigned long _xEkseni1DereceIcinAdimSayisi;
  unsigned long _yEkseni1DereceIcinAdimSayisi;
  void _xBirAdimAt();
  void _yBirAdimAt();
  void _xYonAyarla(byte yon);
  void _yYonAyarla(byte yon);
  void _xTekilHareket(int gidilecekKonumunDerecesi);
  void _yTekilHareket(int gidilecekKonumunDerecesi);
  void _cifteHareket(int xGidilecekKonumunDerecesi, int yGidilecekKonumunDerecesi);
  
  public:
  SM(byte SMXStepPin, byte SMXDirPin, byte SMYStepPin, byte SMYDirPin, byte SMX350DereceLimitPin, byte SMX0DereceLimitPin, byte SMY90DereceLimitPin, byte SMY0DereceLimitPin);
  unsigned long SMXKalibrasyon();
  unsigned long SMYKalibrasyon();
  void yonDegistir(bool kontrol);
  void hizAyarla(int xHiz, int yHiz);
  void git(int xGidilecekKonumunDerecesi, int yGidilecekKonumunDerecesi); 
};
