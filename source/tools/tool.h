/*
 *  tool.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 26.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef __OSS_TOOL_H__
#define __OSS_TOOL_H__
#include "geometry.h"

namespace OSS {
	class World;
	
	class Tool : public Geometry {
	public:
		Pointer<World> world;
	};
}

#endif
