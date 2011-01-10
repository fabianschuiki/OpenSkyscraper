#include "scene.h"

#include "application.h"

using namespace OSS;
using namespace Engine;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

Scene::Scene(EngineCore * engine) : Responder(), engine(engine)
{
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Presentation
//----------------------------------------------------------------------------------------------------

bool Scene::isOnScreen()
{
	return (engine->getScene() == this);
}

void Scene::didMoveOnScreen()
{
	updateViewport();
	updateProjection();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Visual Convenience
//----------------------------------------------------------------------------------------------------

void Scene::updateViewport()
{
	int2 size = engine->application->video->currentMode.resolution;
	glViewport(0, 0, size.x, size.y);
}

void Scene::updateProjection()
{
	int2 size = engine->application->video->currentMode.resolution;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, size.x, 0, size.y, -1, 1);
	glMatrixMode(GL_MODELVIEW);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Event Handling
//----------------------------------------------------------------------------------------------------

void Scene::eventVideoChanged(VideoEvent * event)
{
	updateViewport();
	updateProjection();
}
