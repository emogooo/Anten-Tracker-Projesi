
#include <AccelStepper.h>
AccelStepper s(1, 53, 52); // Motor Arayüz Tipi, Step Pini, Yön Pini

void setup() {
  s.setMaxSpeed(500);
  s.setAcceleration(100);
  s.setSpeed(100);
}

void loop() {
  //if (myStepper.distanceToGo() == 0) 
   // myStepper.moveTo(-myStepper.currentPosition());

  s.move(50);
  s.runToPosition();
  delay(3000);
  s.move(50);
  s.runToPosition();
  delay(3000);
}
