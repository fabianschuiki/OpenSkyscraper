/*
 *  renderer.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 23.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef __OSS_RENDERER_H__
#define __OSS_RENDERER_H__
#include "common.h"
#include "subsystem.h"
#include "camera.h"

namespace OSS {	
	class Renderer : public Subsystem {
	public:		
		int2 size;
		
		Pointer<Node> rootNode;
		Pointer<Camera> camera;
		
		void setRootNode(Node * node);
		void setCamera(Camera * camera);
		
		//Constructor
		Renderer(Engine * engine) : Subsystem(engine) {
			//DEBUG: remove this line, only required to test texture loading
			prepareOpenGL();
		}
		
		void prepareOpenGL();
		void draw();
		void reshape(int2 size);
	};
}

#endif
