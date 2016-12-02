#ifndef COMMAND_H
#define COMMAND_H
#include "Robot.h"

#include <stdlib.h>

class Command{

	public:
		Command();


		virtual void execute();
		virtual bool isFinished();
		virtual void setParallel(bool p);
		virtual bool getParallel();

	private:
		Robot* robot;
		bool running, parallel;
};

#endif
