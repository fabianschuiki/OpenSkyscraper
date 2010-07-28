/*
 *  node.cpp
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 24.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#include "node.h"

using namespace OSS;

Node::~Node()
{
	for (ChildList::iterator child = children.begin(); child != children.end(); child++)
		(*child)->parent = NULL;
}

void Node::attachChild(Spatial* child)
{
	if (!child) return;
	
	//Assert that the child has no parent
	assert(!child->parent);
	
	//Set the child's parent
	child->parent = this;
	
	//Add the child to the list
	children.push_back(child);
}

void Node::detachChild(Spatial* child)
{
	if (!child) return;
	if (child->parent != this) return;
	
	//Reset the child's parent
	child->parent = NULL;
	
	//Remove the child from us
	children.remove(child);
}

bool childrenSortComparison(Spatial* first, Spatial* second)
{
	if (first->appliesToAllSiblings) return true;
	return first->zIndex > second->zIndex;
}

void Node::updateWorldData()
{
	Spatial::updateWorldData();
	children.sort(childrenSortComparison);
	for (ChildList::iterator child = children.begin(); child != children.end(); child++)
		(*child)->updateGeometricState(false);
}

void Node::updateWorldBounds()
{
	bool firstBounds = true;
	for (ChildList::iterator child = children.begin(); child != children.end(); child++) {
		if ((*child)->boundless) continue;
		if (firstBounds) {
			firstBounds = false;
			worldBounds = (*child)->worldBounds;
		} else {
			worldBounds.unify((*child)->worldBounds);
		}
	}
}

void Node::draw()
{
	for (ChildList::reverse_iterator child = children.rbegin(); child != children.rend(); child++)
		(*child)->draw();
}

void Node::reshape(int2 size)
{
	for (ChildList::iterator child = children.begin(); child != children.end(); child++)
		(*child)->reshape(size);
}

bool Node::handleEvent(Event * event)
{
	for (ChildList::iterator child = children.begin(); child != children.end(); child++)
		if ((*child)->handleEvent(event))
			return true;
	return false;
}

std::string Node::dumpGraph(unsigned int indent)
{
	std::string res = Spatial::dumpGraph(indent);
	//res += "\n";
	for (ChildList::iterator child = children.begin(); child != children.end(); child++) {
		res += (*child)->dumpGraph(indent + 1);
		//res += "\n";
	}
	return res;
}
