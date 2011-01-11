//#include "general.h"
#include "game/application.h"
//#include "engine/application.h"

int main(int argc, char *argv[])
{	
	/*OSS::OpenSkyscraper app;
	
	app.prepare();
	app.run();
	app.cleanUp();*/
	
	OSS::Game::Application * app = new OSS::Game::Application;
	app->run();
	app->release();
	
	return 0;
}
