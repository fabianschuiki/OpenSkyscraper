#include "toolboxwindow.h"
#include "../scenes/towerscene.h"

using namespace OSS;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

ToolboxWindow::ToolboxWindow(TowerScene * scene) : Window(), towerScene(scene)
{
	//Initialize the background sprite
	backgroundSprite.color = (color4d){0.85, 0.85, 0.85, 1};
	
	//Initialize the buttons
	initButtons();
	
	//Initial layout
	layout();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Buttons
//----------------------------------------------------------------------------------------------------

void ToolboxWindow::initButtons()
{
	initPauseButton();
	initToolButtons();
	initItemButtons();
	
	updateButtons();
}

void ToolboxWindow::updateButtons()
{
	updatePauseButton();
	updateToolButtons();
	updateItemButtons();
}



void ToolboxWindow::initPauseButton()
{
	//Set the button size
	pauseButton.setRect(rectd(0, 0, 63, 32));
	pauseButton.textureRect.size.x = 63 / 64.0;
}

void ToolboxWindow::initToolButtons()
{
	for (int i = kBulldozerTool; i < kConstructionTool; i++)
		toolButtons[(Tool)i];
	
	for (ToolButtonMap::iterator i = toolButtons.begin(); i != toolButtons.end(); i++) {
		i->second.setRect(rectd(0, 0, 21, 21));
		i->second.textureRect.size.x = (21 / 64.0);
		i->second.textureRect.origin.x = ((int)i->first * 21 / 64.0);
	}
}

void ToolboxWindow::initItemButtons()
{
	for (int i = Item::kNoneType + 1; i < Item::kMaxType; i++)
		itemButtons[(Item::Type)i];
	
	for (ItemButtonMap::iterator i = itemButtons.begin(); i != itemButtons.end(); i++) {
		i->second.setRect(rectd(0, 0, 32, 32));
		i->second.textureRect.size = double2(1.0 / 8, 1.0 / 4);
		
		//Calculate the texture rect origin
		unsigned int sliceIndex = ((int)i->first - 1);
		double2 origin(sliceIndex % 8, 3 - sliceIndex / 8);
		origin *= i->second.textureRect.size;
		i->second.textureRect.origin = origin;
		
		//Fetch the item descriptor
		ItemDescriptor * desc = Item::descriptorForItemType(i->first);
		if (desc)
			groups[desc->group].push_back(i->first);
	}
	
	for (GroupMap::iterator i = groups.begin(); i != groups.end(); i++)
		selectedGroupIndices[i->first] = 0;
}



void ToolboxWindow::updatePauseButton()
{
	if (!towerScene->tower) return;
	
	//Assemble the texture name
	std::string textureName = "simtower/ui/toolbox/";
	textureName += (towerScene->tower->paused ? "play/" : "pause/");
	textureName += (pauseButtonPressed ? "pressed" : "normal");
	
	//Load the texture
	pauseButton.texture = Texture::named(textureName);
}

void ToolboxWindow::updateToolButtons()
{
	std::string baseName = "simtower/ui/toolbox/tools/";
	
	for (ToolButtonMap::iterator i = toolButtons.begin(); i != toolButtons.end(); i++) {
		std::string variantName = (towerScene->getTool() == i->first ? "selected" : "normal");
		i->second.texture = Texture::named(baseName + variantName);
	}
}

void ToolboxWindow::updateItemButtons()
{
	std::string baseName = "simtower/ui/toolbox/construction/";
	
	//Fetch the construction item descriptor
	ItemDescriptor * desc = towerScene->constructionItemDescriptor;
	Item::Type type = (desc ? desc->type : Item::kNoneType);
	
	for (ItemButtonMap::iterator i = itemButtons.begin(); i != itemButtons.end(); i++) {
		bool selected = (type == i->first);
		std::string variantName = (selected ? "selected" : "normal");
		i->second.texture = Texture::named(baseName + variantName);
		if (selected && desc)
			for (int g = 0; g < groups[desc->group].size(); g++)
				if (groups[desc->group][g] == type)
					selectedGroupIndices[desc->group] = g;
	}
	
	layoutItemButtons();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Layout
//----------------------------------------------------------------------------------------------------

void ToolboxWindow::layout()
{
	//Layout the window
	recti frame;
	frame.origin.y = OpenGLCanvas::shared()->currentMode.resolution.y;
	frame.size = int2(63, 180);
	frame.origin.y -= frame.size.y;
	setFrame(frame);
	
	//Position the buttons
	layoutPauseButton();
	layoutToolButtons();
	layoutItemButtons();
}

void ToolboxWindow::layoutPauseButton()
{
	rectd rect = pauseButton.getRect();
	rect.origin.y = getFrame().maxY() - rect.size.y;
	pauseButton.setRect(rect);
}

void ToolboxWindow::layoutToolButtons()
{
	for (ToolButtonMap::iterator i = toolButtons.begin(); i != toolButtons.end(); i++) {
		rectd rect = i->second.getRect();
		rect.origin.x = (int)i->first * rect.size.x;
		rect.origin.y = pauseButton.getRect().minY() - rect.size.y;
		i->second.setRect(rect);
	}
}

void ToolboxWindow::layoutItemButtons()
{
	//Iterate through the groups
	int n = 0;
	for (GroupMap::iterator i = groups.begin(); i != groups.end(); i++, n++) {
		
		//Fetch the vertical item offset
		int offset = selectedGroupIndices[i->first];
		
		//Iterate through the items in the group
		for (int t = 0; t < i->second.size(); t++) {
			Sprite * sprite = &itemButtons[i->second[t]];
			rectd rect = sprite->getRect();
			rect.origin.x = (n % 2) * rect.size.x;
			rect.origin.y = toolButtons[kBulldozerTool].getRect().minY();
			rect.origin.y -= rect.size.y * (1 + t - offset + n / 2);
			sprite->setRect(rect);
			sprite->setHidden(t != offset);
		}
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void ToolboxWindow::draw(rectd visibleRect)
{
	Window::draw(visibleRect);
	
	pauseButton.draw(visibleRect);
	for (ToolButtonMap::iterator i = toolButtons.begin(); i != toolButtons.end(); i++)
		i->second.draw(visibleRect);
	for (ItemButtonMap::iterator i = itemButtons.begin(); i != itemButtons.end(); i++)
		i->second.draw(visibleRect);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Events
//----------------------------------------------------------------------------------------------------

bool ToolboxWindow::eventMouseDown(SDL_Event * event)
{
	int2 point(event->button.x, event->button.y);
	
	//Ignore the event if it didn't happen inside the window
	if (!getFrame().containsPoint(point))
		return false;
	
	return true;
}

bool ToolboxWindow::eventMouseUp(SDL_Event * event)
{
	return false;
}
