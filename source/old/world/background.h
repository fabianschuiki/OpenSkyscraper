/*
 *  background.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 26.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef __OSS_BACKGROUND_H__
#define __OSS_BACKGROUND_H__
#include "node.h"
#include "sky.h"
#include "ground.h"

namespace OSS {
	class World;
	
	class Background : public Node {
		friend class Sky;
		friend class Ground;
		
	public:
		Background();
		
		Pointer<World> world;

	protected:
		//Returns the rect where the background should be drawn
		rectd backgroundRect();
		
		Sky sky;
		Ground ground;
	};
}

#endif
