#include "Robot.h"



// Point current;
// Point prev;

// int thresh = 450;
// int initialDifference = 0;

 //Encoder le(leftEncoder1, leftEncoder2);
 //Encoder re(rightEncoder1, rightEncoder2);

// Servo servo;
Robot* Robot::robot = NULL;

Robot::Robot(){}

Robot* Robot::getInstance(){
  if(robot == NULL) robot = new Robot();
  return robot;
}

void Robot::initializeSubsystems(){
  robot->drivetrain = new DriveTrain();
  robot->fan = new Fan();
  robot->flame = new FlameDetector();
}
