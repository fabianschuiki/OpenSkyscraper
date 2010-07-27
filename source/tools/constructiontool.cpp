/*
 *  constructiontool.cpp
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 26.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#include "constructiontool.h"
#include "world.h"

using namespace OSS;

Tower * ConstructionTool::getTower() {
	if (world)
		return world->tower;
	return NULL;
}

bool ConstructionTool::handleEvent(Event * event) {
	if (event->typeMask & Event::mouseMoved) {
		localOrigin = event->position - parent->worldOrigin;
		updateGeometricState();
		updateConstruction();
		return true;
	}
	if (event->typeMask & Event::mouseDown) {
		if (constructionPossible)
			construct();
		else
			; //play some intelligent error sound or whatever
		return true;
	}
	return false;
}

void ConstructionTool::draw()
{
	Tower * tower = getTower();
	if (!tower) return;
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	if (constructionPossible)
		glColor4f(0, 1, 0, 0.25);
	else
		glColor4f(1, 0, 0, 0.25);
	drawRect(worldConstructionArea);
	glColor4f(1, 1, 1, 0.5);	
	drawFrameRect(worldConstructionArea);
	
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(worldOrigin.x - 10, worldOrigin.y);
	glVertex2f(worldOrigin.x + 10, worldOrigin.y);
	
	glVertex2f(worldOrigin.x, worldOrigin.y - 10);
	glVertex2f(worldOrigin.x, worldOrigin.y + 10);
	glEnd();
}

void ConstructionTool::updateConstruction()
{
	Tower * tower = getTower();
	if (!tower) return;
	
	constructionArea.origin.x = round(localOrigin.x / tower->segmentSize.x - (double)constructionArea.size.x / 2);
	constructionArea.origin.y = round(localOrigin.y / tower->segmentSize.y - (double)constructionArea.size.y / 2);
	
	worldConstructionArea = constructionArea;
	worldConstructionArea.origin *= tower->segmentSize;
	worldConstructionArea.size *= tower->segmentSize;
	worldConstructionArea.origin += parent->worldOrigin;
	
	rejectionReasonMask = 0;
	
	intersectedItems = tower->itemsIntersectingRect(collisionCategory(), constructionArea);
	if (intersectedItems.size() > 0)
		rejectionReasonMask |= AreaOccupiedReason;
	
	recti rectBelow = constructionArea;
	rectBelow.origin.y -= 1;
	rectBelow.size.y = 1;
	if (!tower->isRectEntirelyOccupied(Item::FacilityCategory, rectBelow) && constructionArea.minY() > 0)
		rejectionReasonMask |= NoAdjacentFloorBelowReason;
	
	recti rectAbove = constructionArea;
	rectAbove.origin.y += rectAbove.size.y;
	rectAbove.size.y = 1;
	if (!tower->isRectEntirelyOccupied(Item::FacilityCategory, rectAbove) && constructionArea.maxY() <= 0)
		rejectionReasonMask |= NoAdjacentFloorAboveReason;
	
	rejectionReasonMask |= itemRejectionReasons();
	
	constructionPossible = (rejectionReasonMask == 0);
	/*if (!constructionPossible) {
		OSSObjectLog << "construction invalid, " << rejectionReasonMask << std::endl;
	}*/
}
