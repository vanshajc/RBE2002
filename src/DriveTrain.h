#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H
#include "Encoder.h"
#include "pins.h"
#include "LinkedList.h"

class DriveTrain {
  Encoder le;
  Encoder re;
   public:
  DriveTrain();
  const static int wallDetected = 100;
 void initialize();
 void drive(int left, int right);
 void driveArcade(double forwardSpeed, double turnRate);
 void turnLeft();
 void kill();
 void displayCoordinates();
 Point getDistanceTraveled();
 void updateCoordinates();
 void forward();
  private:
    Point point;
    bool dir = true; //x is true, y is false
    LinkedList<int> segments;


};

#endif
