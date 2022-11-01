#include "SM.h"

SM::SM(byte SMXStepPin, byte SMXDirectionPin, byte SMYStepPin, byte SMYDirectionPin, byte SMXHighDegreeLimitPin, byte SMXLowerDegreeLimitPin, byte SMYHighDegreeLimitPin, byte SMYLowerDegreeLimitPin) {
  _SMXStepPin = SMXStepPin;
  _SMXDirectionPin = SMXDirectionPin;
  _SMYStepPin = SMYStepPin;
  _SMYDirectionPin = SMYDirectionPin;
  _SMXHighDegreeLimitPin = SMXHighDegreeLimitPin;
  _SMXLowerDegreeLimitPin = SMXLowerDegreeLimitPin;
  _SMYHighDegreeLimitPin = SMYHighDegreeLimitPin;
  _SMYLowerDegreeLimitPin = SMYLowerDegreeLimitPin;
  xSetSpeed(500, 250);
  ySetSpeed(500, 250);
}

void SM::_xTakeAStep() {
  digitalWrite(_SMXStepPin, HIGH);
  delayMicroseconds(_xSpeed);
  digitalWrite(_SMXStepPin, LOW);
  delayMicroseconds(_xSpeed);
}

void SM::_yTakeAStep() {
  digitalWrite(_SMYStepPin, HIGH);
  delayMicroseconds(_ySpeed);
  digitalWrite(_SMYStepPin, LOW);
  delayMicroseconds(_ySpeed);
}

void SM::_xDoubleMoveTakeAStep() {
  digitalWrite(_SMXStepPin, HIGH);
  delayMicroseconds(_xSpeed);
  digitalWrite(_SMXStepPin, LOW);
}

void SM::_yDoubleMoveTakeAStep() {
  digitalWrite(_SMYStepPin, HIGH);
  delayMicroseconds(_ySpeed);
  digitalWrite(_SMYStepPin, LOW);
}

void SM::_xSetDirection(byte direction) {
  _SMXDirection = direction;
  digitalWrite(_SMXDirectionPin, _SMXDirection);
}

void SM::_ySetDirection(byte direction) {
  _SMYDirection = direction;
  digitalWrite(_SMYDirectionPin, _SMYDirection);
}

void SM::_changeDirection(bool control) {
  if (control) {
    _xSetDirection(!_SMXDirection);
  } else {
    _ySetDirection(!_SMYDirection);
  }
}

void SM::_xSingularMove(int degreeOfDestination) {
  // Yönün belirlenmesi.
  if (_xPositionDegree < degreeOfDestination) {  // 0dan 100e
    _xSetDirection(HIGH);
  } else {  // 100den 0a
    _xSetDirection(LOW);
  }
  // Kaç derecelik hareket yapacağının belirlenmesi.
  _xDegreeToGo = abs(_xPositionDegree - degreeOfDestination);
  // Atılacak adımın belirlenmesi.
  _xStepToGo = _xDegreeToGo * _xAxisStepCountForOneDegree;
  // Hızların belirlenmesi.
  if (_xDegreeToGo < 5) {  // Kısa mesafe hareket 1 derecelik adım sayısına(880 adım) göre ivmelenir.
    _xAccelerationStepShare = _xAxisStepCountForOneDegree / 4;
    _xSpeedToChangePerStep = 1;
    _xSpeed = _xLowSpeed + (_xAccelerationStepShare * _xSpeedToChangePerStep);
  } else {  // Uzun mesafe hareket 5 derece (4400 adım). 600-600 ivme adım payı.
    _xAccelerationStepShare = 600;
    _xSpeedToChangePerStep = 1;
    _xSpeed = _xHighSpeed + (_xAccelerationStepShare * _xSpeedToChangePerStep);
  }
  _xStepToGo = _xStepToGo - (_xAccelerationStepShare * 2);
  // Hareket fonksiyonu.
  for (int i = 0; i < _xAccelerationStepShare; i++) {
    _xTakeAStep();
    _xSpeed = _xSpeed - _xSpeedToChangePerStep;
  }
  for (unsigned long i = 0; i < _xStepToGo; i++) {
    _xTakeAStep();
  }
  for (int i = 0; i < _xAccelerationStepShare; i++) {
    _xTakeAStep();
    _xSpeed = _xSpeed + _xSpeedToChangePerStep;
  }
  // Konum güncelleme.
  _xPositionDegree = degreeOfDestination;
}

void SM::_ySingularMove(int degreeOfDestination) {
  // Yönün belirlenmesi.
  if (_yPositionDegree < degreeOfDestination) {  // 0dan 100e
    _ySetDirection(HIGH);
  } else {  // 100den 0a
    _ySetDirection(LOW);
  }
  // Kaç derecelik hareket yapacağının belirlenmesi.
  _yDegreeToGo = abs(_yPositionDegree - degreeOfDestination);
  // Atılacak adımın belirlenmesi.
  _yStepToGo = _yDegreeToGo * _yAxisStepCountForOneDegree;
  // Hızların belirlenmesi.
  if (_yDegreeToGo < 5) {  // Kısa mesafe hareket 1/32*50 için 1 derecelik adım sayısına göre ivmelenir.
    _yAccelerationStepShare = _yAxisStepCountForOneDegree / 4;
    _ySpeedToChangePerStep = 1;
    _ySpeed = _yLowSpeed + (_yAccelerationStepShare * _ySpeedToChangePerStep);
  } else {  // Uzun mesafe hareket 1/32*50 için 1200 adım. 600-600 ivme adım payı.
    _yAccelerationStepShare = 600;
    _ySpeedToChangePerStep = 1;
    _ySpeed = _yHighSpeed + (_yAccelerationStepShare * _ySpeedToChangePerStep);
  }
  _yStepToGo = _yStepToGo - (_yAccelerationStepShare * 2);
  // Hareket fonksiyonu.
  for (int i = 0; i < _yAccelerationStepShare; i++) {
    _yTakeAStep();
    _ySpeed = _ySpeed - _ySpeedToChangePerStep;
  }
  for (unsigned long i = 0; i < _yStepToGo; i++) {
    _yTakeAStep();
  }
  for (int i = 0; i < _yAccelerationStepShare; i++) {
    _yTakeAStep();
    _ySpeed = _ySpeed + _ySpeedToChangePerStep;
  }
  // Konum güncelleme.
  _yPositionDegree = degreeOfDestination;
}

void SM::_doubleMove(int xDegreeOfDestination, int yDegreeOfDestination) {
  // Yönlerin belirlenmesi.
  if (_xPositionDegree < xDegreeOfDestination) {  // 0dan 100e
    _xSetDirection(HIGH);
  } else {  // 100den 0a
    _xSetDirection(LOW);
  }
  if (_yPositionDegree < yDegreeOfDestination) {  // 0dan 100e
    _ySetDirection(HIGH);
  } else {  // 100den 0a
    _ySetDirection(LOW);
  }
  // Kaç derecelik hareket yapılacağının belirlenmesi.
  _xDegreeToGo = abs(_xPositionDegree - xDegreeOfDestination);
  _yDegreeToGo = abs(_yPositionDegree - yDegreeOfDestination);
  // Atılacak adımların belirlenmesi.
  _xStepToGo = _xDegreeToGo * _xAxisStepCountForOneDegree;
  _yStepToGo = _yDegreeToGo * _yAxisStepCountForOneDegree;
  //Hızların belirlenmesi.
  if (_xDegreeToGo < 5) {  // Kısa mesafe hareket 1/32*50 için 1 derecelik adım sayısına göre ivmelenir.
    _xAccelerationStepShare = _xAxisStepCountForOneDegree / 4;
    _xSpeedToChangePerStep = 1;
    _xSpeed = _xLowSpeed + (_xAccelerationStepShare * _xSpeedToChangePerStep);
  } else {  // Uzun mesafe hareket 1/32*50 için minimum adım. 600-600 ivme adım payı.
    _xAccelerationStepShare = 600;
    _xSpeedToChangePerStep = 1;
    _xSpeed = _xHighSpeed + (_xAccelerationStepShare * _xSpeedToChangePerStep);
  }
  if (_yDegreeToGo < 5) {  // Kısa mesafe hareket 1/32*50 için 1 derecelik adım sayısına göre ivmelenir.
    _yAccelerationStepShare = _yAxisStepCountForOneDegree / 4;
    _ySpeedToChangePerStep = 1;
    _ySpeed = _yLowSpeed + (_yAccelerationStepShare * _ySpeedToChangePerStep);
  } else {  // Uzun mesafe hareket 1/32*50 için 1200 adım. 600-600 ivme adım payı.
    _yAccelerationStepShare = 600;
    _ySpeedToChangePerStep = 1;
    _ySpeed = _yHighSpeed + (_yAccelerationStepShare * _ySpeedToChangePerStep);
  }
  // Hareket fonksiyonu.
  _xNumberOfStepsTaken = 0;
  _yNumberOfStepsTaken = 0;
  while (((_xStepToGo - _xNumberOfStepsTaken) > 0 && _xMoveControl) || ((_yStepToGo - _yNumberOfStepsTaken) > 0 && _yMoveControl)) {
    if ((_xStepToGo - _xNumberOfStepsTaken) > 0  && _xMoveControl) {
      if (_xNumberOfStepsTaken < _xAccelerationStepShare) {
        _xSpeed = _xSpeed - _xSpeedToChangePerStep;
      } else if (_xStepToGo - _xNumberOfStepsTaken < _xAccelerationStepShare) {
        _xSpeed = _xSpeed + _xSpeedToChangePerStep;
      }
      _xDoubleMoveTakeAStep();
      _xNumberOfStepsTaken++;
    } else {
      if (_xSpeed > _ySpeed) {
        _xSpeed = _xSpeed - _xSpeedToChangePerStep;
      }
      delayMicroseconds(_xSpeed);
    }
    if ((_yStepToGo - _yNumberOfStepsTaken) > 0 && _yMoveControl) {
      if (_yNumberOfStepsTaken < _yAccelerationStepShare) {
        _ySpeed = _ySpeed - _ySpeedToChangePerStep;
      } else if ((_yStepToGo - _yNumberOfStepsTaken) < _yAccelerationStepShare) {
        _ySpeed = _ySpeed + _ySpeedToChangePerStep;
      }
      _yDoubleMoveTakeAStep();
      _yNumberOfStepsTaken++;
    } else {
      if (_ySpeed > _xSpeed) {
        _ySpeed = _ySpeed - _ySpeedToChangePerStep;
      }
      delayMicroseconds(_ySpeed);
    }
  }
  // Konum güncelleme.
  _xPositionDegree = xDegreeOfDestination;
  _yPositionDegree = yDegreeOfDestination;
}

void SM::SMXCalibration() {
  _xSetDirection(HIGH);
  _xSpeed = 30;
  _xAxisStepCount = 0;
  while (digitalRead(_SMXHighDegreeLimitPin) == LOW) {  // Yönden kaynaklı problem olabilir
    _xTakeAStep();
  }
  _xSetDirection(!_SMXDirection);
  while (digitalRead(_SMXLowerDegreeLimitPin) == LOW) {
    _xTakeAStep();
    _xAxisStepCount++;
  }
  _xSetDirection(!_SMXDirection);
  for (int i = 0; i < 500; i++) {  // Limit Butona baskı yapmaması için 500 adım geri döner. Burada ki 500 adım kör noktadır.
    _xTakeAStep();
  }
  _xAxisStepCountForOneDegree = (_xAxisStepCount - 1000) / 310;
  _xPositionDegree = 0;
  _xMoveControl = true;
}

void SM::SMYCalibration() {
  _ySetDirection(HIGH);
  _ySpeed = 30;
  _yAxisStepCount = 0;
  while (digitalRead(_SMYHighDegreeLimitPin) == LOW) {  // Yönden kaynaklı problem olabilir. Yukarı hareket ediyor.
    _yTakeAStep();
  }
  _ySetDirection(!_SMYDirection);
  while (digitalRead(_SMYLowerDegreeLimitPin) == LOW) {  // Aşağı hareket ediyor.
    _yTakeAStep();
    _yAxisStepCount++;
  }
  _yStepCounter = 0;
  _ySetDirection(!_SMYDirection);
  while (digitalRead(_SMYLowerDegreeLimitPin) == HIGH) {
    _yTakeAStep();
    _yStepCounter++;
  }
  for (int i = 0; i < 500; i++) {  // Limit Butona baskı yapmaması için 1000 adım geri döner. Burada ki 1000 adım kör noktadır.
    _yTakeAStep();
  }
  _yStepCounter = _yStepCounter + 500;
  _ySetDirection(!_SMYDirection);
  for (int i = 0; i < _yStepCounter - 1000; i++) {  // Limit Butona baskı yapmaması için 1000 adım geri döner. Burada ki 1000 adım kör noktadır.
    _yTakeAStep();
  }
  _yAxisStepCountForOneDegree = (_yAxisStepCount - 2000) / 125;
  _yPositionDegree = 0;
  _yMoveControl = true;
}

void SM::xSetSpeed(int lowSpeed, int highSpeed) {
  _xLowSpeed = lowSpeed;
  _xHighSpeed = highSpeed;
}

void SM::ySetSpeed(int lowSpeed, int highSpeed) {
  _yLowSpeed = lowSpeed;
  _yHighSpeed = highSpeed;
}

void SM::emergencyStop(bool isMotorX) {
  if (isMotorX) {
    _xMoveControl = false;
    _xNumberOfStepsTaken = 1;
    _xStepToGo = 1;
    _xAccelerationStepShare = 0;
  } else {
    _yMoveControl = false;
    _yNumberOfStepsTaken = 1;
    _yStepToGo = 1;
    _yAccelerationStepShare = 0;
  }
}

void SM::recalibration(bool isMotorX, bool isHighLimit, int lastDestination) {
  _changeDirection(isMotorX);
  if (isMotorX) {
    _xSpeed = 30;
    _xStepCounter = 0;
    if (isHighLimit) {
      while (digitalRead(_SMXHighDegreeLimitPin) == HIGH) {
        _xTakeAStep();
        _xStepCounter++;
      }
      _xPositionDegree = 309;
    } else {
      while (digitalRead(_SMXLowerDegreeLimitPin) == HIGH) {
        _xTakeAStep();
        _xStepCounter++;
      }
      _xPositionDegree = 0;
    }
    for (int i = 0; i < 500; i++) {
      _xTakeAStep();
    }
    _xStepCounter = _xStepCounter + 500;
    _changeDirection(isMotorX);
    for (int i = 0; i < _xStepCounter - 500; i++) {
      _xTakeAStep();
    }
    _xMoveControl = true;
  } else {
    _ySpeed = 30;
    _yStepCounter = 0;
    if (isHighLimit) {
      while (digitalRead(_SMYHighDegreeLimitPin) == HIGH) {
        _yTakeAStep();
        _yStepCounter++;
      }
      _yPositionDegree = 124;
    } else {
      while (digitalRead(_SMYLowerDegreeLimitPin) == HIGH) {
        _yTakeAStep();
        _yStepCounter++;
      }
      _yPositionDegree = 0;
    }
    for (int i = 0; i < 500; i++) {
      _yTakeAStep();
    }
    _yStepCounter = _yStepCounter + 500;
    _changeDirection(isMotorX);
    for (int i = 0; i < _yStepCounter - 1000; i++) {
      _yTakeAStep();
    }
    _yMoveControl = true;
  }
}

void SM::move(int xDegreeOfDestination, int yDegreeOfDestination) {
  if (xDegreeOfDestination > 309 || xDegreeOfDestination < 0 || yDegreeOfDestination > 124 || yDegreeOfDestination < 0) {
    return;
  }

  if (xDegreeOfDestination != _xPositionDegree && yDegreeOfDestination != _yPositionDegree) {
    _doubleMove(xDegreeOfDestination, yDegreeOfDestination);
  } else if (xDegreeOfDestination != _xPositionDegree) {
    _xSingularMove(xDegreeOfDestination);
  } else if (yDegreeOfDestination != _yPositionDegree) {
    _ySingularMove(yDegreeOfDestination);
  }
}