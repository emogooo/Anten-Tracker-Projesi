#include "Arduino.h"

class SM{
  private:
  byte _SMXStepPin;
  byte _SMXDirPin;
  byte _SMYStepPin;
  byte _SMYDirPin;
  byte _SMXLimit1Pin;
  byte _SMXLimit2Pin;
  byte _SMYLimit1Pin;
  byte _SMYLimit2Pin;
  byte _SMXDir;
  byte _SMYDir;
  int _xHiz;
  int _yHiz;
  int _xHizTutucu;
  int _yHizTutucu;
  unsigned int _xEkseniAdimSayisi;
  unsigned int _yEkseniAdimSayisi;
  int _xKonumDerecesi;
  int _yKonumDerecesi;
  int _xGidilecekDereceSayisi;
  int _yGidilecekDereceSayisi;
  unsigned int _xGidilecekAdimSayisi;
  unsigned int _yGidilecekAdimSayisi;
  unsigned int _xGidilenAdimSayisi;
  unsigned int _yGidilenAdimSayisi;
  unsigned int _xGidilecekNihaiAdimSayisi;
  unsigned int _yGidilecekNihaiAdimSayisi;
  int _xHareketIvmeAdimPayi;
  int _yHareketIvmeAdimPayi;
  int _xHareketIvmeAdimBasinaDegisecekHiz;
  int _yHareketIvmeAdimBasinaDegisecekHiz;
  unsigned int _xEkseni1DereceIcinAdimSayisi;
  unsigned int _yEkseni1DereceIcinAdimSayisi;
  void _baslangic();
  void _xBirAdimAt();
  void _yBirAdimAt();
  void _xYonAyarla(byte yon);
  void _yYonAyarla(byte yon);
  void _xTekilHareket(int gidilecekKonumunDerecesi);
  void _yTekilHareket(int gidilecekKonumunDerecesi);
  void _cifteHareket(int xGidilecekKonumunDerecesi, int yGidilecekKonumunDerecesi);
  
  public:
  SM(byte SMXStepPin, byte SMXDirPin, byte SMYStepPin, byte SMYDirPin, byte SMXLimit1Pin, byte SMXLimit2Pin, byte SMYLimit1Pin, byte SMYLimit2Pin);
  void hizAyarla(int xHiz, int yHiz);
  void git(unsigned int xGidilecekKonumunDerecesi, unsigned int yGidilecekKonumunDerecesi);
  void xAdimSay();
  void yAdimSay();
};
