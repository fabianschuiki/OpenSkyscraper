#include "scene.h"
#include "core.h"

using namespace OSS;


//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Visual Convenience
//----------------------------------------------------------------------------------------------------

void Scene::updateViewport()
{
	int2 size = OpenGLCanvas::shared()->currentMode.resolution;
	glViewport(0, 0, size.x, size.y);
}

void Scene::updateProjection()
{
	int2 size = OpenGLCanvas::shared()->currentMode.resolution;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, size.x, 0, size.y, -1, 1);
	glMatrixMode(GL_MODELVIEW);
}

void Scene::eventVideoModeChanged()
{
	OSSObjectLog;
	updateViewport();
	updateProjection();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Notifications
//----------------------------------------------------------------------------------------------------

void Scene::onMoveOnScreen()
{
	updateViewport();
	updateProjection();
}
