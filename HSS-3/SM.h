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
  unsigned int _xHiz;
  unsigned int _yHiz;
  unsigned int _xHizTutucu;
  unsigned int _yHizTutucu;
  unsigned int _xEkseniAdimSayisi;
  unsigned int _yEkseniAdimSayisi;
  unsigned int _xKonumDerecesi;
  unsigned int _yKonumDerecesi;
  unsigned int _xGidilecekDereceSayisi;
  unsigned int _yGidilecekDereceSayisi;
  unsigned long _xGidilecekAdimSayisi;
  unsigned long _yGidilecekAdimSayisi;
  unsigned long _xGidilenAdimSayisi;
  unsigned long _yGidilenAdimSayisi;
  unsigned int _xHareketIvmeAdimPayi;
  unsigned int _yHareketIvmeAdimPayi;
  unsigned int _xHareketIvmeAdimBasinaDegisecekHiz;
  unsigned int _yHareketIvmeAdimBasinaDegisecekHiz;
  unsigned long _xEkseni1DereceIcinAdimSayisi;
  unsigned long _yEkseni1DereceIcinAdimSayisi;
  void _xBirAdimAt();
  void _yBirAdimAt();
  void _xYonAyarla(byte yon);
  void _yYonAyarla(byte yon);
  void _xTekilHareket(unsigned int gidilecekKonumunDerecesi);
  void _yTekilHareket(unsigned int gidilecekKonumunDerecesi);
  void _cifteHareket(unsigned int xGidilecekKonumunDerecesi, unsigned int yGidilecekKonumunDerecesi);
  
  public:
  SM(byte SMXStepPin, byte SMXDirPin, byte SMYStepPin, byte SMYDirPin, byte SMX350DereceLimitPin, byte SMX0DereceLimitPin, byte SMY90DereceLimitPin, byte SMY0DereceLimitPin);
  void hizAyarla(unsigned int xHiz, unsigned int yHiz);
  void git(unsigned int xGidilecekKonumunDerecesi, unsigned int yGidilecekKonumunDerecesi);
  void SMXKalibrasyon();
  void SMYKalibrasyon();
};
