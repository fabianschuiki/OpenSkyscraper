#include "construction.h"

#include "../../scenes/game.h"
#include "../../items/item.h"
#include "tools.h"
#include "tower.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

ConstructionTool::ConstructionTool(ToolsUI * ui) : Tool(ui),
updateTemplateRectIfNeeded(this, &ConstructionTool::updateTemplateRect, &updateIfNeeded)
{	
	//Select the lobby item by default
	setItemType(kLobbyType);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Item
//----------------------------------------------------------------------------------------------------

ItemType ConstructionTool::getItemType()
{
	return itemType;
}

void ConstructionTool::setItemType(ItemType type)
{
	if (itemType != type) {
		itemType = type;
		setItemDescriptor(Item::descriptorForItemType(type));
	}
}

ItemDescriptor * ConstructionTool::getItemDescriptor()
{
	return itemDescriptor;
}

void ConstructionTool::setItemDescriptor(ItemDescriptor * descriptor)
{
	if (itemDescriptor != descriptor) {
		itemDescriptor = descriptor;
		updateTemplateRectIfNeeded.setNeeded();
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Template
//----------------------------------------------------------------------------------------------------

double2 ConstructionTool::getTemplateCenter()
{
	return templateCenter;
}

void ConstructionTool::setTemplateCenter(double2 center)
{
	if (templateCenter != center) {
		templateCenter = center;
		updateTemplateRectIfNeeded.setNeeded();
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

void ConstructionTool::update()
{
	//Update the template rect if required
	updateTemplateRectIfNeeded();
}

void ConstructionTool::updateTemplateRect()
{
	//Set the template's size
	if (getItemDescriptor())
		templateRect.size = getItemDescriptor()->minUnit;
	
	//In order to position the template, we have to convert the template center which is in world
	//coordinates to cell coordinates. The first step is to convert the template rect's size to
	//world coordinates so we can do the centering properly.
	double2 size = ui->getTower()->structure->cellToWorld(templateRect.size);
	
	//Now calculate the origin of the template in world coordinates.
	double2 origin = templateCenter - (size / 2);
	
	//Now convert the origin to cell coordinates.
	templateRect.origin = ui->getTower()->structure->worldToCell(origin);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void ConstructionTool::draw(rectd dirtyRect)
{
	//Draw the template rect. To do this we first need to convert the template rect to world co-
	//ordinates.
	rectd world = ui->getTower()->structure->cellToWorld(templateRect);
	
	//Since we're drawing in the UI and everything is in window coordinates, we have to convert the
	//world coordinates to window coordinates.
	rectd window = ui->getScene()->worldToWindow(world);
	
	//Now we have to fix the window rect. Remember that we want to draw its frame and since drawing
	//a line requires the center of a pixel instead of a corner of it, we have to inset the entire
	//rect by 0.5 (half a pixel).
	window.inset(double2(0.5, 0.5));
	
	//Now we may draw the frame around the rect
	glColor4f(1, 1, 1, 0.75);
	Engine::Texture::unbind();
	glBegin(GL_LINE_STRIP);
	glVertex2d(window.minX(), window.minY());
	glVertex2d(window.maxX(), window.minY());
	glVertex2d(window.maxX(), window.maxY());
	glVertex2d(window.minX(), window.maxY());
	glVertex2d(window.minX(), window.minY());
	glEnd();
	
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Event Handling
//----------------------------------------------------------------------------------------------------

bool ConstructionTool::eventMouseDown(Core::MouseButtonEvent * event)
{
	return false;
}

bool ConstructionTool::eventMouseUp(Core::MouseButtonEvent * event)
{
	return false;
}

bool ConstructionTool::eventMouseMoved(Core::MouseMovedEvent * event)
{
	//Set the template's center to be where the mouse is
	setTemplateCenter(ui->getScene()->windowToWorld(event->position, true));
	
	return true;
}
