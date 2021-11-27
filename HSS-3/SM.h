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
  int _xEkseniAdimSayisi;
  int _yEkseniAdimSayisi;
  int _xKonumDerecesi;
  int _yKonumDerecesi;
  int _xGidilecekDereceSayisi;
  int _yGidilecekDereceSayisi;
  int _xGidilecekAdimSayisi;
  int _yGidilecekAdimSayisi;
  int _xGidilenAdimSayisi;
  int _yGidilenAdimSayisi;
  int _xGidilecekNihaiAdimSayisi;
  int _yGidilecekNihaiAdimSayisi;
  int _xHareketIvmeAdimPayi;
  int _yHareketIvmeAdimPayi;
  int _xHareketIvmeAdimBasinaDegisecekHiz;
  int _yHareketIvmeAdimBasinaDegisecekHiz;
  float _xEkseni1DereceIcinAdimSayisi;
  float _yEkseni1DereceIcinAdimSayisi;
  void _baslangic();
  void _xBirAdimAt();
  void _yBirAdimAt();
  void _xAdimSay();
  void _yAdimSay();
  void _xYonAyarla(byte yon);
  void _yYonAyarla(byte yon);
  void _xTekilHareket(int gidilecekKonumunDerecesi);
  void _yTekilHareket(int gidilecekKonumunDerecesi);
  void _cifteHareket(int xGidilecekKonumunDerecesi, int yGidilecekKonumunDerecesi);
  
  public:
  SM(byte SMXStepPin, byte SMXDirPin, byte SMYStepPin, byte SMYDirPin, byte SMXLimit1Pin, byte SMXLimit2Pin, byte SMYLimit1Pin, byte SMYLimit2Pin);
  void hizAyarla(int xHiz, int yHiz);
  void git(int xGidilecekKonumunDerecesi, int yGidilecekKonumunDerecesi);
};
