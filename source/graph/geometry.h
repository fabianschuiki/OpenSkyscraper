/*
 *  geometry.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 26.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef __OSS_GEOMETRY_H__
#define __OSS_GEOMETRY_H__
#include "common.h"
#include "spatial.h"
#include "opengl.h"

namespace OSS {
	class Geometry : public Spatial {
	public:
		//Simple drawing functions
		static void drawRect(rectd rect);
		static void drawTexturedRect(rectd rect, rectd texrect);
		static void drawFrameRect(rectd rect);
		
	protected:
		virtual void updateWorldBounds() {}
	};
}

#endif
