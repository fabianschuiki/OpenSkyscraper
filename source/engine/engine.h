/*
 *  engine.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 23.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef __OSS_ENGINE_H__
#define __OSS_ENGINE_H__
#include "object.h"
#include "pointer.h"
#include "renderer.h"
#include "tower.h"
#include "node.h"
#include "gameui.h"
#include "input.h"
#include "world.h"

namespace OSS {
	extern const std::string ImageResourceType;
	
	enum Mode {
		MenuMode,
		GameMode
	};
	
	class Engine : public Object {
	public:		
		//Subsystems
		Pointer<Renderer> renderer;
		Pointer<Input> input;
		
		//Root nodes
		Node menuRoot;
		Pointer<Node> gameRoot;
		
		//Game nodes
		Pointer<World> world;
		
		//UIs
		Pointer<GameUI> gameUI;
		
		//Mode
		unsigned int mode;
		void setMode(unsigned int mode);
		Node * currentRootNode();
		Camera * currentCamera();
		
		Engine();
		~Engine();
		
		//Tower being played
		Pointer<Tower> tower;
		void setTower(Tower * tower);
		
		//Paths
		static std::string pathForResource(std::string type, std::string name);
	};
}

#endif
