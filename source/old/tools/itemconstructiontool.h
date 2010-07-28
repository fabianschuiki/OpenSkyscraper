/*
 *  itemconstructiontool.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 26.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef __OSS_ITEMCONSTRUCTIONTOOL_H__
#define __OSS_ITEMCONSTRUCTIONTOOL_H__
#include "constructiontool.h"
#include "lobbyitem.h"

namespace OSS {
	template <class T>
	class ItemConstructionTool : public ConstructionTool {
	public:
		Pointer<T> templateItem;
		ItemConstructionTool() : ConstructionTool() {
			templateItem = new T;			
			constructionArea.size.x = templateItem->minSize.x;
			constructionArea.size.y = templateItem->minSize.y;
		}
		
		void construct() {
			Tower * tower = getTower();
			assert(tower);
			
			Item * item = new T;
			item->frame.origin = constructionArea.origin;
			tower->constructItem(item);
		}
		
		Item::Category collisionCategory() {
			return templateItem->category;
		}
		
		unsigned int itemRejectionReasons() {
			unsigned int reasons = 0;
			if (constructionArea.minY() == 0 && typeid(T) != typeid(LobbyItem))
				reasons |= OnlyLobbyOnGroundFloorReason;
			
			unsigned int specific = templateItem->constructionRejectionReasonsForRect(constructionArea);
			reasons |= (specific << 8);
			
			return reasons;
		}
	};
}

#endif
