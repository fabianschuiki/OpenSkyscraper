/*
 *  ground.cpp
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 26.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#include "ground.h"
#include "background.h"

using namespace OSS;


void Ground::draw() {
	//Cut off the section of the background rect with positive y coordinates
	rectd rect = background->backgroundRect();
	rect.size.y = -rect.origin.y;
	rect.origin += worldOrigin;
	
	//DEBUG: draw the debug sky
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3f(0.6, 0.4, 0.2);
	drawRect(rect);
}
