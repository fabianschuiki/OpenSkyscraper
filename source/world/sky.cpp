/*
 *  sky.cpp
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 26.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#include "sky.h"
#include "background.h"

using namespace OSS;


void Sky::draw() {
	//Cut off the section of the background rect with negative y coordinates
	rectd rect = background->backgroundRect();
	rect.size.y += rect.origin.y;
	rect.origin.y = 0;
	rect.origin += worldOrigin;
	
	//DEBUG: draw the debug sky
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3f(0, 0.5, 1);
	drawRect(rect);
}
