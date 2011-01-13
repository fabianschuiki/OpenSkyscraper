#include "engine/base/autoreleasequeue.h"
#include "game/application.h"

using namespace OSS;


int main(int argc, char *argv[])
{
	AutoreleaseQueue queue;
	
	OpenSkyscraper * app = new OpenSkyscraper;
	app->run();
	
	return 0;
}
