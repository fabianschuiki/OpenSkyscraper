/*
 *  world.cpp
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 26.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#include "world.h"
#include "itemconstructiontool.h"
#include "lobbyitem.h"
#include "officeitem.h"

using namespace OSS;

void World::selectTool(Tool * tool)
{
	if (currentTool) {
		camera->detachChild(currentTool);
		currentTool->world = NULL;
	}
	currentTool = tool;
	if (currentTool) {
		currentTool->world = this;
		camera->attachChild(currentTool);
	}
}

World::World()
{
	//Initialize the background
	background = new Background;
	background->world = this;
	background->zIndex = -100;
	
	//Initialize the camera
	camera = new Camera;
	attachChild(camera);
	
	//Assemble the camera tree
	camera->attachChild(background);
	
	//Create the list of construction tools
	constructionTools.push_back(new ItemConstructionTool<LobbyItem>);
	constructionTools.push_back(new ItemConstructionTool<OfficeItem>);
}

World::~World()
{
	camera = NULL;
	background = NULL;
	
	tower = NULL;
}

void World::setTower(Tower * tower)
{
	if (this->tower) camera->detachChild(this->tower);
	this->tower = tower;
	if (this->tower) {
		camera->attachChild(this->tower);
		camera->updateGeometricState();
	}
}
