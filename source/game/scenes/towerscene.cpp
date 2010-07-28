#include "towerscene.h"
#include "../../classes.h"

using namespace OSS;


//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void TowerScene::advance(double dt)
{
	//Update the visible rect
	int2 canvasSize = OpenGLCanvas::shared()->currentMode.resolution;
	visibleRect.origin = POI - canvasSize / 2;
	visibleRect.size = canvasSize;
	
	//Calculate the visible cells rect
	visibleCells = tower->convertWorldToCellRect(visibleRect);
	
	//Find the items to draw
	visibleFacilities.clear();
	visibleTransports.clear();
	for (int x = visibleCells.minX(); x < visibleCells.maxX(); x++) {
		for (int y = visibleCells.minY(); y < visibleCells.maxY(); y++) {
			Tower::Cell * cell = tower->getCell(int2(x, y));
			if (!cell) continue;
			
			visibleFacilities.insert(cell->facility);
			visibleTransports.insert(cell->transport);
		}
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Rendering
//----------------------------------------------------------------------------------------------------

void TowerScene::render()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	
	//Translate modelview matrix so that the POI is centered on screen
	glTranslated(-visibleRect.origin.x, -visibleRect.origin.y, 0);
	
	//Render the items
	renderFacilities();
	renderTransports();
	
	//Draw a red cross at the origin of the world
	glBindTexture(GL_TEXTURE_RECTANGLE_EXT, 0);
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(-9, 0);
	glVertex2f(10, 0);
	glVertex2f(0, -9);
	glVertex2f(0, 10);
	glEnd();
	
	OpenGLCanvas::shared()->swapBuffers();
}

void TowerScene::renderFacilities()
{
}

void TowerScene::renderTransports()
{
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark OpenGL State
//----------------------------------------------------------------------------------------------------

void TowerScene::onMoveOnScreen()
{
	Scene::onMoveOnScreen();
	
	glEnable(GL_TEXTURE_RECTANGLE_EXT);
	glEnable(GL_BLEND);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void TowerScene::onMoveOffScreen()
{
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_RECTANGLE_EXT);
	
	Scene::onMoveOffScreen();
}
