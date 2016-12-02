// Robot.ino
// This is the main file.
// Van is amazing
//change from Morgan
#include "pins.h"
#include "Robot.h"
#include "Encoder.h"
#include "PID_v1.h"
#include "LiquidCrystal.h"
#include <Servo.h>
#include "math.h"
#include "Command.h"
#include "DriveTrain.h"
#include "Fan.h"
#include "FlameDetector.h"
#include "pins.h"
#include "Robot.h"
#include "Scheduler.h"
#include "WallFollow.h"
#include "DriveToCandle.h"

Scheduler* scheduler = Scheduler::getInstance();
Robot* robot = Robot::getInstance();

void setup() {
  robot->initializeSubsystems();
  scheduler->addParallel( new DriveToCandle(.5));
}

void loop() {
  scheduler->run();
}
