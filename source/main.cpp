#include "Application.h"

int main(int argc, char * argv[])
{
	OT::Application * app = new OT::Application(argc, argv);
	int exitCode = app->run();
	delete app;
	app = NULL;
	return exitCode;
}
