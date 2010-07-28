/*
 *  camera.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 26.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef __OSS_CAMERA_H__
#define __OSS_CAMERA_H__
#include "node.h"
#include "opengl.h"

namespace OSS {
	class Camera : public Node {
	public:		
		/**
		 * The camera size is set through the reshape() function, called from within the
		 * renderer class. The camera adjusts the projection matrix and updates its
		 * geometric state, so the entire scene graph is aware of the new camera bounds.
		 */
		int2 size;
		void reshape(int2 size) {
			this->size = size;
			updateGeometricState();
		}
		
		/**
		 * Applies the camera to the current OpenGL matrix by translating it by the negative
		 * of the camera position and half of the viewport size (so the camera origin actually
		 * describes the center of the viewport inside the world.
		 */
		/*void apply() {
			glTranslatef(-worldOrigin.x + size.x / 2, -worldOrigin.y + size.y / 2, 0);
		}*/
		void updateWorldData() {
			worldOrigin = -localOrigin + double2(size.x / 2, size.y / 2);
			worldOriginIsCurrent = true;
			Node::updateWorldData();
		}
		
		/**
		 * The camera is embedded somewhere in the scene graph. Therefore we use the draw()
		 * method to apply the transformation to all subsequent drawing calls.
		 */
		//void draw() { /*apply();*/ }
		
	protected:
		void updateWorldBounds() {
			worldBounds.size = double2(size.x, size.y);
			worldBounds.origin = worldOrigin - double2(size.x / 2, size.y / 2);
		}
	};
}

#endif
