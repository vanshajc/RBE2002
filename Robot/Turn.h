#include "Command.h"
#include "Robot.h"

class Turn : public Command{
	public:
		Turn();

		void execute();
		bool isFinished();
};
