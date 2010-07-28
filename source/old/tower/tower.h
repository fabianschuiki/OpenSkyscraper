/*
 *  tower.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 24.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef __OSS_TOWER_H__
#define __OSS_TOWER_H__
#include "common.h"
#include "node.h"
#include "item.h"

namespace OSS {	
	class Tower : public Node {
		//This will change, just to show what belongs into this class…
		double funds;
		unsigned short stars;
		
	public:
		double2 segmentSize;
		double ceilingHeight;
		
		//Item grouping nodes
		Pointer<Node> facilityGroup;
		Pointer<Node> transportationGroup;
		
		//Item lists
		typedef std::list< Pointer<Item> > ItemList;
		ItemList facilityItems;
		ItemList transportationItems;
		
		ItemList * itemListForCategory(Item::Category category);
		Node * groupNodeForCategory(Item::Category category);
		
		Tower();
		virtual ~Tower();
		
		//DEBUG: draws a red cross at the origin of the tower - just for debugging!
		void draw();
		
		void addItem(Pointer<Item> item);
		void removeItem(Pointer<Item> item);
		
		void constructItem(Pointer<Item> item);
		void destroyItem(Pointer<Item> item);
		
		ItemList itemsIntersectingRect(Item::Category category, recti rect);
		ItemList itemsBelowPoint(Item::Category category, int2 point);
		bool isRectEntirelyOccupied(Item::Category category, recti rect);
		
		void collapseAdjacentItems();
	};
}

#endif
