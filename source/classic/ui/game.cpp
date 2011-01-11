#include "game.h"

#include "../scenes/game.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

GameUI::GameUI(GameScene * scene) : Responder(), scene(scene),
updateRootViewIfNeeded(this, &GameUI::updateRootView, &updateIfNeeded)
{
	//Initialize the root view which will contain all the subviews.
	rootView = new GUI::View;
	
	//Initialize the control window
	controlWindow = new ControlWindow(this);
	controlWindow->setFrameOrigin(rootView->getFrame().maxXmaxY() - controlWindow->getFrameSize());
	controlWindow->setAutoresizingMask(GUI::View::MaxXFixed | GUI::View::MaxYFixed);
	rootView->addSubview(controlWindow);
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
}

void GameUI::updateRootView()
{
	//Set the root view's frame to cover the entire screen
	rectd rect;
	rect.size = Engine::Video::getCurrent()->currentMode.resolution;
	rootView->setFrame(rect);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void GameUI::draw(rectd dirtyRect)
{
	//Ask the root view to draw itself
	rootView->draw(dirtyRect);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Event Handling
//----------------------------------------------------------------------------------------------------

void GameUI::eventVideoChanged(Engine::VideoEvent * event)
{
	//Call the superclass's implementation of the function first
	Responder::eventVideoChanged(event);
	
	//Mark the root view so its frame gets adjustet to the new conditions
	updateRootViewIfNeeded.setNeeded();
}
