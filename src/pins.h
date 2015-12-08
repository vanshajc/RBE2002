#ifndef pins_h
#define pins_h

#include "Arduino.h"

typedef struct {
 double x;
 double y;
} Point;

enum DigitalInputs {
  kLeftEncoderA = 2,
  kLeftEncoderB = 3,
  kRightEncoderA = 19,
  kRightEncoderB = 18
};

enum AnalogInputs {
  kLeftMotorA = 8, //previously leftMotor1
  kLeftMotorB = 9,
  kRightMotorA = 11,
  kRightMotorB = 10,
  kFrontIR = 2,
  //kLeftFrontIR = 1,
  //kLeftBackIR = 2,
  kRightFrontIR = 3,
  //kRightBackIR = 3,
  kFrontFlame = 8,
  kSideFlame = 1,
  kServoPin = 4,
  kFanPin = 11,
  kUserButton = 24
};
const double kP = 0.1;
const int kThresh = 500;
#endif
