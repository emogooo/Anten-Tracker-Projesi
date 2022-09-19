#include "Arduino.h"

class SM{
  private:
  byte _SMXStepPin;
  byte _SMXDirPin;
  byte _SMYStepPin;
  byte _SMYDirPin;
  byte _SMXEndLimitPin;
  byte _SMXStartLimitPin;
  byte _SMYEndLimitPin;
  byte _SMYStartLimitPin;
  byte _SMXDir;
  byte _SMYDir;
  int _xHiz;
  int _yHiz;
  int _xYavasHiz;
  int _xHizliHiz;
  int _yYavasHiz;
  int _yHizliHiz;
  int _xEkseniDerecesi;
  int _yEkseniDerecesi;
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
  void _xCifteHareketBirAdimAt();
  void _yCifteHareketBirAdimAt();
  void _xYonAyarla(byte yon);
  void _yYonAyarla(byte yon);
  void _xTekilHareket(int gidilecekKonumunDerecesi);
  void _yTekilHareket(int gidilecekKonumunDerecesi);
  void _cifteHareket(int xGidilecekKonumunDerecesi, int yGidilecekKonumunDerecesi);
  
  public:
  SM(byte SMXStepPin, byte SMXDirPin, byte SMYStepPin, byte SMYDirPin, byte SMXEndLimitPin, byte SMX0DereceLimitPin, byte SMYEndLimitPin, byte SMYStartLimitPin, unsigned int xEkseniDerecesi, unsigned int yEkseniDerecesi);
  unsigned long SMXKalibrasyon();
  unsigned long SMYKalibrasyon();
  void yonDegistir(bool kontrol);
  void xHizAyarla(int yavasHiz, int hizliHiz);
  void yHizAyarla(int yavasHiz, int hizliHiz);
  void git(int xGidilecekKonumunDerecesi, int yGidilecekKonumunDerecesi); 
};
