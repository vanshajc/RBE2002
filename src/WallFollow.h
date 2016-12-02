#include "Command.h"
#include "Robot.h"

class WallFollow : public Command{
	public:
		WallFollow(int _speed);
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
		void initialize();
		void execute();
		void end();
		bool isFinished();
		void setParallel(bool p);
		bool getParallel();
		void setRunning(bool p);
		bool getRunning();
		bool cycle();
		void start();
	private:
		Robot* robot;
		int speed;
};
