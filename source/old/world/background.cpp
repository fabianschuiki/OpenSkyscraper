/*
 *  background.cpp
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 26.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#include "background.h"
#include "world.h"

using namespace OSS;


Background::Background()
{
	//Since we adjust to the bounds of the world and don't want to
	//influence it, we are boundless
	boundless = true;
	
	sky.background = this;
	ground.background = this;
	
	//Assemble the background tree
	attachChild(&sky);
	attachChild(&ground);
}

rectd Background::backgroundRect()
{
	rectd rect = world->worldBounds;
	rect.origin -= worldOrigin;
	return rect;
}
