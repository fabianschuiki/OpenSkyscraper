//#include "general.h"
#include "game/openskyscraper.h"
//#include "engine/application.h"

int main(int argc, char *argv[])
{	
	/*OSS::OpenSkyscraper app;
	
	app.prepare();
	app.run();
	app.cleanUp();*/
	
	OSS::OpenSkyscraper * app = new OSS::OpenSkyscraper;
	app->run();
	app->release();
	
	return 0;
}
