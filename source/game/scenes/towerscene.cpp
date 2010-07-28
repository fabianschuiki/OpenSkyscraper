#include "towerscene.h"
#include "../../classes.h"

using namespace OSS;


void TowerScene::render()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	
	OpenGLCanvas::shared()->swapBuffers();
}
