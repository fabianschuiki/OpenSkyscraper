#include "general.h"
#include "game/openskyscraper.h"

int main(int argc, char *argv[])
{
	OSS::OpenSkyscraper app;
	
	app.prepare();
	app.run();
	app.cleanUp();
	
	return 0;
}
