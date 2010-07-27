/*
 *  item.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 26.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef __OSS_ITEM_H__
#define __OSS_ITEM_H__
#include "geometry.h"

namespace OSS {
	class Tower;
	
	class Item : public Geometry {
	public:		
		enum Category {
			FacilityCategory,
			TransportationCategory
		};
		
		int2 unitSize;
		int2 minSize;
		Category category;
		bool isCollapsible;
		
		Item();
		
		virtual unsigned int constructionRejectionReasonsForRect(recti rect);
		
		Pointer<Tower> tower;
		
		recti frame;
		double2 worldSize;
		
		virtual const GLfloat * debugGLColor() { static const GLfloat c[3] = {0.7, 0.7, 0.7}; return c; }
		virtual void draw();
		
	protected:
		virtual void updateWorldData();
	};
}

#endif
