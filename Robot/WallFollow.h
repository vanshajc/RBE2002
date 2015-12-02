#include "Command.h"
#include "Robot.h"

class WallFollow : public Command{
	public:
		WallFollow();

		void execute();
		bool isFinished();
};