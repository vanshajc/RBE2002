

class Command{

	public:
		Command();
		Command(String name);


		virtual void execute();
		virtual bool isFinished();

};