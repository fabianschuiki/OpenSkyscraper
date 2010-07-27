/*
 *  world.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 26.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef __OSS_WORLD_H__
#define __OSS_WORLD_H__
#include "node.h"
#include "background.h"
#include "tower.h"
#include "camera.h"
#include "constructiontool.h"

namespace OSS {
	class World : public Node {
	public:		
		Pointer<Background> background;
		Pointer<Camera> camera;
		
		std::vector< Pointer<ConstructionTool> > constructionTools;
		Pointer<Tool> currentTool;
		void selectTool(Tool * tool);
		
		World();
		virtual ~World();
		
		Pointer<Tower> tower;
		void setTower(Tower * tower);
	};
}

#endif
