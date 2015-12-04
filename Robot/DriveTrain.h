#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H
#include "Encoder/Encoder.h"

class DriveTrain {
   public:
  DriveTrain(int leftMotorPin1, int leftMotorPin2,
   int rightMotorPin1, int rightMotorPin2, int leftEncoderPin1,
 int leftEncoderPin2, int rightEncoderPin1, int rightEncoderPin2);
 void initialize();
 void drive(int left, int right);
 void driveArcade(double forwardSpeed, double turnRate);
  private:
    const int LEFT_ENC_PIN1;
    const int RIGHT_ENC_PIN1;
    const int LEFT_ENC_PIN2;
    const int RIGHT_ENC_PIN2;
    const Encoder le(LEFT_ENC_PIN1,LEFT_ENC_PIN2);
    const Encoder re(RIGHT_ENC_PIN1,RIGHT_ENC_PIN2);
};

#endif
