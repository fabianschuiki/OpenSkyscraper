#include "simulationtask.h"
#include "../core.h"

using namespace OSS;


void SimulationTask::update()
{
	Scene * scene = Engine::shared()->scene;
	if (scene)
		scene->advance(Engine::shared()->dt);
}
