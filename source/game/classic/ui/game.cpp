#include "game.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

GameUI::GameUI(GameScene * scene) : scene(scene),
updateRootViewIfNeeded(this, &GameUI::updateRootView, &updateIfNeeded)
{
	//Initialize the root view which will contain all the subviews.
	rootView = new View;
	
	//Initialize the tools subsystem and adjust its updateIfNeeded conditional to propagate a need
	//for updates to the game UI.
	tools = new ToolsUI(this);
	tools->updateIfNeeded.parent = &updateIfNeeded;
	
	//Initialize the control window
	controlWindow = new ControlWindow(this);
	controlWindow->setFrameOrigin(rootView->getFrame().maxXmaxY() - controlWindow->getFrameSize());
	controlWindow->setAutoresizingMask(View::MaxXFixed | View::MaxYFixed);
	rootView->addSubview(controlWindow);
	
	//Initialize the tools window
	toolsWindow = new ToolsWindow(this);
	toolsWindow->setAutoresizingMask(View::MinXFixed | View::MinYFixed);
	rootView->addSubview(toolsWindow);
}

Tower * GameUI::getTower()
{
	return scene->tower;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void GameUI::advance(double dt)
{
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

void GameUI::update()
{	
	//Update the root view if required. This usually is the case if the Video mode changed and the
	//root view needs to be resized.
	updateRootViewIfNeeded();
	
	//Update the tools subsystem
	tools->updateIfNeeded();
}

void GameUI::updateRootView()
{
	//Set the root view's frame to cover the entire screen
	rectd rect;
	rect.size = Video::shared()->currentMode.resolution;
	rootView->setFrame(rect);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void GameUI::draw(rectd dirtyRect)
{
	//Draw the tools
	tools->draw(dirtyRect);
	
	//Ask the root view to draw itself
	rootView->draw(dirtyRect);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Event Sending
//----------------------------------------------------------------------------------------------------

bool GameUI::sendEventToNextResponders(OSS::Event * event)
{
	if (rootView && rootView->sendEvent(event)) return true;
	if (tools && tools->sendEvent(event)) return true;
	return SceneObject::sendEventToNextResponders(event);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Event Handling
//----------------------------------------------------------------------------------------------------

void GameUI::eventVideoChanged(VideoEvent * event)
{
	//Call the superclass's implementation of the function first
	SceneObject::eventVideoChanged(event);
	
	//Mark the root view so its frame gets adjustet to the new conditions
	updateRootViewIfNeeded.setNeeded();
}
