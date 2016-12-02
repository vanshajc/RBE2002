#ifndef COMMAND_H
#define COMMAND_H

#include <stdlib.h>

class Command{

	public:
		Command();

		virtual void initialize();
		virtual void execute();
		virtual void end();
		virtual bool isFinished();
		virtual bool cycle();
		void setParallel(bool p);
		bool getParallel();
	 	bool getRunning();
		void start();

	private:
		bool initialized;
		bool running;
		bool parallel;
};

#endif
