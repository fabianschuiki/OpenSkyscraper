/*
 *  sprite.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 24.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef __OSS_SPRITE_H__
#define __OSS_SPRITE_H__
#include "common.h"
#include "opengl.h"
#include "geometry.h"

namespace OSS {
	class Sprite : public Geometry {
	public:
		double2 size;
		
		virtual void draw() {}
		
		virtual bool shouldHandleMouseEvent(Event * event) {
			return rectd(worldOrigin, size).containsPoint(event->position);
		}
		
	protected:
		virtual void updateWorldBounds();
	};
}

#endif
