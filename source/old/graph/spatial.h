/*
 *  spatial.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 24.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef __OSS_SPATIAL_H__
#define __OSS_SPATIAL_H__
#include "common.h"
#include "object.h"
#include "pointer.h"
#include "event.h"

namespace OSS {
	/**
	 * Forward-declare classes that will be friend to the spatial class.
	 */
	class Node;
	class Geometry;
	
	/**
	 * Spatial class which provides the superclass for everything that can
	 * be positioned anywhere in space, like nodes, sprites, etc..
	 */
	class Spatial : public Object {
		//Declare some friends which will have access to the protected stuff
		friend class Node;
		friend class Geometry;
		
	public:
		bool ignore;
		
		//Drawing order
		int zIndex;
		bool appliesToAllSiblings;	//causes the Spatial to be drawn before all it's siblings.
									//this is useful for Cameras and similar classes.
		
		double2 localOrigin;
		double2 worldOrigin;
		bool worldOriginIsCurrent;
		
		rectd worldBounds;
		bool boundless;
		
		Spatial();
		
		void updateGeometricState(bool initiator = true);
		void updateBoundsState();
		
		virtual void draw() {};
		virtual void reshape(int2 size) {};
		
		//Event handling
		virtual bool handleEvent(Event * event);
		virtual bool shouldHandleMouseEvent(Event * event) { return false; }
		
		//Mouse events
		virtual bool onMouseDown(double2 position, unsigned short button) { return false; }
		virtual bool onMouseDragged(double2 position, unsigned short button) { return false; }
		virtual bool onMouseUp(double2 position, unsigned short button) { return false; }
		
		virtual std::string dumpGraph(unsigned int indent = 0);
		
	protected:
		//The spatial's parent, which is another spatial
		Pointer<Spatial> parent;
		void setParent(Spatial* parent);
		
		//Updating mechanism used to calculate world data and bounds from local information
		virtual void updateWorldData();
		virtual void updateWorldBounds() = 0;
		void propagateBoundsToRoot();
	};
}

#endif
