/*
 *  node.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 24.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef __OSS_NODE_H__
#define __OSS_NODE_H__
#include "spatial.h"
#include "common.h"

namespace OSS {
	
	class Node : public Spatial {
	public:
		virtual ~Node();
		
		void attachChild(Spatial* child);
		void detachChild(Spatial* child);
		
		virtual void draw();
		virtual void reshape(int2 size);
		
		virtual bool handleEvent(Event * event);
		
		virtual std::string dumpGraph(unsigned int indent = 0);
		
		typedef std::list< Pointer<Spatial> > ChildList;
		ChildList children;
		
	protected:		
		virtual void updateWorldData();
		virtual void updateWorldBounds();
	};
}

#endif
