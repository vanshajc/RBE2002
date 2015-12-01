#ifndef pins_h
#define pins_h

#define leftMotor1 10
#define rightMotor1 9

#define leftMotor2 11
#define rightMotor2 8


#define leftEncoder1 2
#define leftEncoder2 3
#define rightEncoder1 19
#define rightEncoder2 20

#define frontRightIR 3
#define backIR 2
#define frontIR 2


typedef struct {
 double x;
 double y;
} Point;


enum DigitalInputs {
  kLeftMotorA = 10,
  kLeftMotorB = 11,
  kRightMotorA = 9,
  kRightMotorB = 8,
  kLeftEncoderA = 2,
  kLeftEncoderB = 3,
  kRightEncoderA = 19,
  kRightEncoderB = 20
};

enum AnalogInputs {
  kFrontIR = 2,
  kBackIR = 3,
};

#define stopPin 24
#define servoPin 4
#define fan 5




#endif
