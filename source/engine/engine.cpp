/*
 *  engine.cpp
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 23.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#include "common.h"
#include "engine.h"

using namespace OSS;

//Resource type constants
const std::string ImageResourceType = "images";

Engine::Engine()
{
	//Initialize the subsystems
	renderer = new Renderer(this);
	input = new Input(this);
	
	//Initialize the UIs
	gameUI = new GameUI();
	gameUI->zIndex = 100;
	
	//Initialize the game node tree
	gameRoot = new Node;
	world = new World;
	
	//Assemble the game node tree
	gameRoot->attachChild(gameUI);
	gameRoot->attachChild(world);
	gameRoot->updateGeometricState();
	
	//Initialize an empty tower
	setTower(new Tower);
	
	//Switch to game mode
	setMode(GameMode);
	
	//Dump the scene graph
	std::cout << currentRootNode()->dumpGraph();
}

Engine::~Engine()
{	
	//Release the subsystems
	renderer = NULL;
	input = NULL;
	
	//Release the UIs
	gameUI = NULL;
	
	//Release the game node tree
	gameRoot = NULL;
	world = NULL;
	
	//Release the tower
	tower = NULL;
}

void Engine::setMode(unsigned int mode)
{
	this->mode = mode;
	if (renderer) {
		renderer->setRootNode(currentRootNode());
		renderer->setCamera(currentCamera());
	}
	Node * node = currentRootNode();
	if (node)
		node->updateGeometricState();
}

Node * Engine::currentRootNode()
{
	if (mode == GameMode) return gameRoot;
	return NULL;
}

Camera * Engine::currentCamera()
{
	if (mode == GameMode) return world->camera;
	return NULL;
}

void Engine::setTower(Tower * tower)
{
	this->tower = tower;
	if (world)
		world->setTower(tower);
}
