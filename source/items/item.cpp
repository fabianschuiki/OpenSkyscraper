/*
 *  item.cpp
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 26.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#include "item.h"
#include "tower.h"

using namespace OSS;

unsigned int Item::constructionRejectionReasonsForRect(recti rect) { return 0; }

Item::Item()
{
	category = FacilityCategory;
	isCollapsible = false;
	unitSize.x = 1;
	unitSize.y = 1;
	minSize = unitSize;
}

void Item::updateWorldData()
{
	if (tower) {
		worldSize = frame.size * tower->segmentSize;
		localOrigin = frame.origin * tower->segmentSize;
	}
	Geometry::updateWorldData();
}

void Item::draw()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3fv(debugGLColor());
	Geometry::drawRect(rectd(worldOrigin, worldSize));
	glColor4f(0, 0, 0, 0.5);
	Geometry::drawFrameRect(rectd(worldOrigin, worldSize));
}
