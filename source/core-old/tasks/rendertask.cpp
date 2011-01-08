#include "rendertask.h"
#include "../core.h"

using namespace OSS;


void RenderTask::update()
{
	Scene * scene = Engine::shared()->scene;
	if (scene)
		scene->render();
}
