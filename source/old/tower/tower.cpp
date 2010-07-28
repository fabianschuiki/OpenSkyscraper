/*
 *  tower.cpp
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 24.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#include "opengl.h"
#include "tower.h"
#include "item.h"
#include "geometry.h"

using namespace OSS;

Tower::Tower()
{
	//Load some default world dimension constants
	segmentSize.x = 8;
	segmentSize.y = 24 + 12;
	ceilingHeight = 12;
	
	facilityGroup = new Node;
	transportationGroup = new Node;
	
	facilityGroup->zIndex = 0;
	transportationGroup->zIndex = 1;
	
	attachChild(facilityGroup);
	attachChild(transportationGroup);
}

Tower::~Tower()
{
	facilityGroup = NULL;
	transportationGroup = NULL;
}

void Tower::draw()
{
	Node::draw();
	
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(worldOrigin.x - 10, worldOrigin.y);
	glVertex2f(worldOrigin.x + 10, worldOrigin.y);
	
	glVertex2f(worldOrigin.x, worldOrigin.y - 10);
	glVertex2f(worldOrigin.x, worldOrigin.y + 10);
	glEnd();
	
	//Draw the bounds
	glColor4f(0, 1, 0, 0.25);
	Geometry::drawFrameRect(worldBounds);
}

/*std::list<Item*> itemsIntersectedByRect(recti rect)
{
}*/

Tower::ItemList * Tower::itemListForCategory(Item::Category category)
{
	switch (category) {
		case Item::FacilityCategory: return &facilityItems; break;
		case Item::TransportationCategory: return &transportationItems; break;
	}
	return NULL;
}

Node * Tower::groupNodeForCategory(Item::Category category)
{
	switch (category) {
		case Item::FacilityCategory: return facilityGroup; break;
		case Item::TransportationCategory: return transportationGroup; break;
	}
	return NULL;
}

void Tower::addItem(Pointer<Item> item)
{
	OSSObjectLog << item->description() << std::endl;
	
	Tower::ItemList * items = itemListForCategory(item->category);
	Node * group = groupNodeForCategory(item->category);
	
	item->tower = this;
	items->push_back(item);
	group->attachChild(item);

	if (item->isCollapsible)
		collapseAdjacentItems();
	group->updateGeometricState();
}

void Tower::removeItem(Pointer<Item> item)
{
	OSSObjectLog << item->description() << std::endl;
	
	Tower::ItemList * items = itemListForCategory(item->category);
	Node * group = groupNodeForCategory(item->category);
	
	item->tower = NULL;
	items->remove(item);
	group->detachChild(item);
	group->updateGeometricState();
}

void Tower::constructItem(Pointer<Item> item)
{
	addItem(item);	
	std::cout << dumpGraph();
}

void Tower::destroyItem(Pointer<Item> item)
{
	removeItem(item);
}

Tower::ItemList Tower::itemsIntersectingRect(Item::Category category, recti rect)
{
	ItemList foundItems;
	ItemList * items = itemListForCategory(category);
	for (ItemList::iterator item = items->begin(); item != items->end(); item++)
		if (rect.intersectsRect((*item)->frame))
			foundItems.push_back(*item);
	return foundItems;
}

Tower::ItemList Tower::itemsBelowPoint(Item::Category category, int2 point)
{
	ItemList foundItems;
	ItemList * items = itemListForCategory(category);
	for (ItemList::iterator item = items->begin(); item != items->end(); item++)
		if ((*item)->frame.containsPoint(point))
			foundItems.push_back(*item);
	return foundItems;
}

bool Tower::isRectEntirelyOccupied(Item::Category category, recti rect)
{
	bool occupied = true;
	for (int y = rect.minY(); y < rect.maxY() && occupied; y++) {
		for (int x = rect.minX(); x < rect.maxX() && occupied; x++) {
			ItemList items = itemsBelowPoint(category, int2(x, y));
			if (items.size() == 0)
				occupied = false;
			else
				for (ItemList::iterator item = items.begin(); item != items.end(); item++)
					x = std::max<int>((*item)->frame.maxX() - 1, x);
		}
	}
	return occupied;
}

void Tower::collapseAdjacentItems()
{
	OSSFuncLog();
	ItemList * items = itemListForCategory(Item::FacilityCategory);
	for (ItemList::iterator item = items->begin(); item != items->end(); item++) {
		for (ItemList::iterator possibleNeighbor = items->begin(); possibleNeighbor != items->end(); possibleNeighbor++) {
			Pointer<Item> maintainedItem = *item;
			Pointer<Item> collapsedItem = *possibleNeighbor;
			bool touchesLeft = maintainedItem->frame.minX() == collapsedItem->frame.maxX();
			bool touchesRight = maintainedItem->frame.maxX() == collapsedItem->frame.minX();
			if (touchesLeft || touchesRight) {
				//Extend the maintained item's frame to cover the other item
				//and remove the collapsed item from the tower.
				maintainedItem->frame.unify(collapsedItem->frame);
				removeItem(collapsedItem);
				
				//Restart the iteration since we encountered some crashes when
				//continuing with the modified list.
				item = items->begin();
				possibleNeighbor = items->begin();
			}
		}
	}
}

