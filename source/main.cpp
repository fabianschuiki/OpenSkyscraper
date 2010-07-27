#include "general.h"
#include "core/application.h"

int main(int argc, char *argv[])
{
	OSS::Application app;
	
	app.prepare();
	app.run();
	app.cleanUp();
	
	return 0;
}
