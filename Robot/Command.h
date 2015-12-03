#include <stdlib.h>

class Command{

	public:
		Command();


		virtual void execute();
		virtual bool isFinished();

};
