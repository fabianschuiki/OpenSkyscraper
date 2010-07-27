/*
 *  sky.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 26.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef __OSS_SKY_H__
#define __OSS_SKY_H__
#include "geometry.h"

namespace OSS {
	class Background;
	
	class Sky : public Geometry {
	public:
		Pointer<Background> background;
		void draw();
	};
}

#endif
