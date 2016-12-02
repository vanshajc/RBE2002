#include "Command.h"
#include "Robot.h"

class CheckFlame : public Command{
	public:
		CheckFlame();

		void execute();
		bool isFinished();
	private:
		Robot* robot;
};
