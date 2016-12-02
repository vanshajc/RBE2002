#include "DriveToCandle.h"

DriveToCandle::DriveToCandle(int _speed) : Command() {
	robot = Robot::getInstance();
	parallel = false;
	running = false;
	initialized = false;
	_speed = speed;
}

// virtual void initialize();
// virtual void execute();
// virtual void end();
// virtual bool isFinished();
// virtual bool cycle();

void DriveToCandle::initialize() {
}

void DriveToCandle::execute(){
	robot->drivetrain->driveArcade(0.8,0);
}

bool DriveToCandle::isFinished(){

	return (false);
}

void DriveToCandle::end(){
	robot->drivetrain->drive(0,0);
}
