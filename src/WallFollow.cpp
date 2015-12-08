#include "WallFollow.h"

WallFollow::WallFollow(int _speed) : Command(){
	robot = Robot::getInstance();
	speed = _speed;
}

// virtual void initialize();
// virtual void execute();
// virtual void end();
// virtual bool isFinished();
// virtual void setParallel(bool p);
// virtual bool getParallel();
// virtual void setRunning(bool p);
// virtual bool getRunning();
// virtual bool cycle();
// virtual void start();

void WallFollow::initialize() {
}

void WallFollow::execute(){
	double val = kP*(analogRead(kRightFrontIR)- kThresh);
	if(analogRead(kFrontIR) < robot->drivetrain->wallDetected){
	if(analogRead(kRightFrontIR) < 100){
		robot->drivetrain->drive(220,150);
	}
	robot->drivetrain->driveArcade(speed,val);
}else{
	robot->drivetrain->turnLeft();
}

}
void WallFollow::end(){
	robot->drivetrain->drive(0,0);
}

bool WallFollow::isFinished(){

	return true;
}
