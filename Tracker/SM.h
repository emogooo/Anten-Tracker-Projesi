#include "Arduino.h"

class SM {
private:
  byte _SMXStepPin;
  byte _SMXDirectionPin;
  byte _SMYStepPin;
  byte _SMYDirectionPin;
  byte _SMXHighDegreeLimitPin;
  byte _SMXLowerDegreeLimitPin;
  byte _SMYHighDegreeLimitPin;
  byte _SMYLowerDegreeLimitPin;
  byte _SMXDirection;
  byte _SMYDirection;
  int _xSpeed;
  int _ySpeed;
  int _xLowSpeed;
  int _xHighSpeed;
  int _yLowSpeed;
  int _yHighSpeed;
  unsigned long _xAxisStepCount;
  unsigned long _yAxisStepCount;
  unsigned long _xAxisStepCountForOneDegree;
  unsigned long _yAxisStepCountForOneDegree;
  int _xPositionDegree;
  int _yPositionDegree;
  int _xDegreeToGo;
  int _yDegreeToGo;
  unsigned long _xStepToGo;
  unsigned long _yStepToGo;
  unsigned long _xNumberOfStepsTaken;
  unsigned long _yNumberOfStepsTaken;
  int _xAccelerationStepShare;
  int _yAccelerationStepShare;
  int _xSpeedToChangePerStep;
  int _ySpeedToChangePerStep;
  void _xTakeAStep();
  void _yTakeAStep();
  void _xDoubleMoveTakeAStep();
  void _yDoubleMoveTakeAStep();
  void _xSetDirection(byte direction);
  void _ySetDirection(byte direction);
  void _xSingularMove(int degreeOfDestination);
  void _ySingularMove(int degreeOfDestination);
  void _doubleMove(int xDegreeOfDestination, int yDegreeOfDestination);

public:
  SM(byte SMXStepPin, byte SMXDirectionPin, byte SMYStepPin, byte SMYDirectionPin, byte SMXHighDegreeLimitPin, byte SMXLowerDegreeLimitPin, byte SMYHighDegreeLimitPin, byte SMYLowerDegreeLimitPin);
  void SMXCalibration();
  void SMYCalibration();
  void changeDirection(bool control);
  void xSetSpeed(int lowSpeed, int highSpeed);
  void ySetSpeed(int lowSpeed, int highSpeed);
  void move(int xDegreeOfDestination, int yDegreeOfDestination);
};