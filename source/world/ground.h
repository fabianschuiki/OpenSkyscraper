/*
 *  ground.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 26.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef __OSS_GROUND_H__
#define __OSS_GROUND_H__
#include "geometry.h"

namespace OSS {
	class Background;
	
	class Ground : public Geometry {
	public:
		Pointer<Background> background;
		void draw();
	};
}

#endif
