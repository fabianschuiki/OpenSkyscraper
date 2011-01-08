//#include "general.h"
//#include "game/openskyscraper.h"
#include "engine/application.h"

int main(int argc, char *argv[])
{	
	/*OSS::OpenSkyscraper app;
	
	app.prepare();
	app.run();
	app.cleanUp();*/
	
	OSS::Engine::Application * app = new OSS::Engine::Application;
	app->run();
	app->release();
	
	return 0;
}
