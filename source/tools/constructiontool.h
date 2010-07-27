/*
 *  constructiontool.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 26.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef __OSS_CONSTRUCTIONTOOL_H__
#define __OSS_CONSTRUCTIONTOOL_H__
#include "tool.h"
#include "tower.h"

namespace OSS {
	class ConstructionTool : public Tool {
	public:
		Tower * getTower();
		
		bool handleEvent(Event * event);
		void draw();
		void updateConstruction();
		
		virtual void construct() = 0;
		virtual Item::Category collisionCategory() = 0;
		virtual unsigned int itemRejectionReasons() = 0;
		
		Tower::ItemList intersectedItems;
		enum RejectionReason {
			AreaOccupiedReason				= (1 << 0),
			NoAdjacentFloorBelowReason		= (1 << 1),
			NoAdjacentFloorAboveReason		= (1 << 2),
			OnlyLobbyOnGroundFloorReason	= (1 << 3),
			
			ItemSpecificReasons				= (0xFFFF << 8)
		};
		unsigned int rejectionReasonMask;
		bool constructionPossible;
		
	protected:
		recti constructionArea;
		rectd worldConstructionArea;
	};
}

#endif
