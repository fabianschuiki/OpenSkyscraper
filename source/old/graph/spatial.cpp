/*
 *  spatial.cpp
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 24.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#include "spatial.h"

using namespace OSS;

Spatial::Spatial()
{
	ignore = false;
	
	zIndex = 0;
	appliesToAllSiblings = false;
	worldOriginIsCurrent = false;
	
	boundless = false;
}

void Spatial::setParent(Spatial* parent)
{
	this->parent = parent;
}

void Spatial::updateGeometricState(bool initiator)
{
	updateWorldData();
	updateWorldBounds();
	if (initiator)
		propagateBoundsToRoot();
}

void Spatial::updateBoundsState()
{
	updateWorldBounds();
	propagateBoundsToRoot();
}

void Spatial::propagateBoundsToRoot()
{
	if (parent) {
		parent->updateWorldBounds();
		parent->propagateBoundsToRoot();
	}
}

void Spatial::updateWorldData()
{
	if (!worldOriginIsCurrent) {
		if (parent)
			worldOrigin = localOrigin + parent->worldOrigin;
		else
			worldOrigin = localOrigin;
	}
	worldOriginIsCurrent = false;
}

std::string Spatial::dumpGraph(unsigned int indent)
{
	std::string res;
	for (unsigned int i = 0; i < indent; i++) {
		if (i == indent - 1)
			res += "+-";
		else
			res += "| ";
	}
	res += description();
	res += "\n";
	return res;
}

bool Spatial::handleEvent(Event * event)
{
	//Mouse events
	if (event->typeMask & Event::mouse) {
		if (!shouldHandleMouseEvent(event)) return false;
		double2 local = event->position - worldOrigin;
		if (event->typeMask & Event::mouseDown) return onMouseDown(local, event->button);
		if (event->typeMask & Event::mouseDragged) return onMouseDragged(local, event->button);
		if (event->typeMask & Event::mouseUp) return onMouseUp(local, event->button);
	}
	return false;
}
