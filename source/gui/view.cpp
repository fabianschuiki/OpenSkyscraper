#include "view.h"

using namespace OSS;
using namespace GUI;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Hierarchy
//----------------------------------------------------------------------------------------------------

View * View::getSuperview()
{
	return superview;
}

void View::setSuperview(View * superview)
{
	if (this->superview != superview) {
		willMoveToSuperview(superview);
		this->superview = superview;
		didMoveToSuperview();
	}
}

void View::removeFromSuperview()
{
	if (superview)
		superview->removeSubview(this);
}

bool View::isDescendantOf(View * view)
{
	if (this == view)
		return true;
	if (superview)
		return superview->isDescendantOf(view);
	return false;
}

View * View::getRootView()
{
	if (!superview)
		return this;
	return superview->getRootView();
}

const View::List & View::getSubviews()
{
	return subviews;
}

void View::setSubviews(const List& subviews)
{
	if (this->subviews != subviews) {
		this->subviews = subviews;
	}
}

void View::addSubview(View * subview)
{
	if (!subview) return;
	
	//Notify subclasses about the view being added.
	willAddSubview(subview);
	
	//Add the view to the end of the subviews list and set its superview to ourself.
	subviews.push_back(subview);
	subview->setSuperview(this);
	
	//Notify subclasses that we did add the view.
	didAddSubview(subview);
}

void View::addSubview(View * subview, OrderingMode positioned, View * relativeTo)
{
	if (!subview) return;
	
	//Notify subclasses that we're going to insert a new view.
	willAddSubview(subview);
	
	//Search for the view we want to be inserted relative to
	List::iterator it;
	for (it = subviews.begin(); it != subviews.end(); it++)
		if ((View *)*it == relativeTo)
			break;
	
	//The iterator now either points at the relativeTo view or at the end of the subviews list. If
	//we want to insert before that location, we can simply use the iterator in the insert()
	//function. If we want to inserted after the view, we have to increase the interator if we're
	//not at the end of the list already.
	if (positioned == Above && it != subviews.end())
		it++;
	
	//Insert the view and set its superview to ourself.
	subviews.insert(it, subview);
	subview->setSuperview(this);
	
	//Notify subclasses that a new view was added.
	didAddSubview(subview);
}


void View::removeSubview(View * subview)
{
	if (!subview) return;
	
	//Autoretain the view, i.e. retain and autorelease it to make sure it is valid throughout this
	//function. This is important since remove() will actually release our reference to the view
	//which might cause the view to be freed.
	subview->autoretain();
	
	//Notify subclasses that we're going to remove a subview.
	willRemoveSubview(subview);
	
	//Set the view's superview to NULL first, so it might respond to the process of moving away as
	//long as it's acutally still in its superview. After that, remove the subview from the list.
	subview->setSuperview(NULL);
	subviews.remove(subview);
	
	//Notify subclasses that we actually removed the subview.
	didRemoveSubview(subview);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Frame
//----------------------------------------------------------------------------------------------------

const rectd & View::getFrame()
{
	return frame;
}

void View::setFrame(const rectd & frame)
{
	if (this->frame != frame) {
		//Keep the old origin and size around for notifications
		double2 oldOrigin = this->frame.origin;
		double2 oldSize = this->frame.size;
		
		//Find out what parts of the frame are actually changing
		bool originChanges = (oldOrigin != frame.origin);
		bool sizeChanges = (oldSize != frame.size);
		
		//Notify subclasses about what we're going to change
		willChangeFrame(frame);
		if (originChanges) willChangeFrameOrigin(frame.origin);
		if (sizeChanges) willChangeFrameSize(frame.size);
		
		//Store the new frame
		this->frame = frame;
		
		//Notify subclasses about what was changed
		didChangeFrame(frame);
		if (originChanges) didChangeFrameOrigin(oldOrigin);
		if (sizeChanges) didChangeFrameSize(oldSize);
	}
}

const double2 & View::getFrameOrigin()
{
	return frame.origin;
}

void View::setFrameOrigin(const double2 & origin)
{
	setFrame(rectd(origin, frame.size));
}

const double2 & View::getFrameSize()
{
	return frame.size;
}

void View::setFrameSize(const double2 & size)
{
	setFrame(rectd(frame.origin, size));
}

/**
 * The default implementation of didChangeFrameSize() will actually ask the view to layout its sub-
 * views with the given old size. Subclasses overriding this function should consider calling this
 * default implementation somewhere in their subclass.
 */
void View::didChangeFrameSize(const double2 & oldSize)
{
	layoutSubviewsWithOldSize(oldSize);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Layout
//----------------------------------------------------------------------------------------------------

unsigned int View::getAutoresizingMask()
{
	return autoresizingMask;
}

void View::setAutoresizingMask(unsigned int autoresizingMask)
{
	this->autoresizingMask = autoresizingMask;
}

void View::layoutSubviewsWithOldSize(double2 oldSize)
{
	//Iterate through the subviews and ask each to layout
	for (List::iterator it = subviews.begin(); it != subviews.end(); it++)
		(*it)->layoutWithOldSuperviewSize(oldSize);
}

void View::layoutWithOldSuperviewSize(double2 oldSize)
{
	if (!getSuperview()) return;
	
	//Fetch our autoresizing mask
	unsigned int mask = getAutoresizingMask();
	
	//Setup a rect for modification
	rectd frame = getFrame();
	rectd rect = frame;
	
	//Get the current superview size
	double2 size = getSuperview()->getFrameSize();
	
	//Calculate the margins before the superview changed its size
	double minXbefore = frame.minX();
	double minYbefore = frame.minY();
	double maxXbefore = oldSize.x - frame.maxX();
	double maxYbefore = oldSize.y - frame.maxY();
	
	//In case both min and max are fixed, the width/height has to be altered
	if (mask & MinXFixed && mask & MaxXFixed)
		rect.size.x = size.x - minXbefore - maxXbefore;
	if (mask & MinYFixed && mask & MaxYFixed)
		rect.size.y = size.y - minYbefore - maxYbefore;
	
	//In case max is fixed, we have to reposition the rect
	if (mask & MaxXFixed)
		rect.origin.x = size.x - maxXbefore - rect.size.x;
	if (mask & MaxYFixed)
		rect.origin.y = size.y - maxYbefore - rect.size.y;
	
	//Note that we don't have to adjust the origin in case the min is fixed!
	
	//Use this new rect
	setFrame(rect);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void View::draw(rectd dirtyRect)
{
	//Iterate through the subviews
	for (List::iterator it = subviews.begin(); it != subviews.end(); it++) {
		
		//Check if the subview's frame intersects the dirty rect and thus should be drawn.
		if ((*it)->getFrame().intersectsRect(dirtyRect)) {
			
			//Translate the OpenGL matrix to the view's frame origin
			double2 origin = (*it)->getFrameOrigin();
			glPushMatrix();
			glTranslated(origin.x, origin.y, 0);
			
			//Draw the view with the dirty rect translated to the view's coordinate space.
			(*it)->draw(rectd(dirtyRect.origin - origin, dirtyRect.size));
			
			//Restore the previous OpenGL matrix
			glPopMatrix();
		}
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Event Sending
//----------------------------------------------------------------------------------------------------

bool View::sendEventToNextResponders(Base::Event * event)
{
	//Iterate through the events and send the event to each until one is able to handle it.
	for (List::iterator it = subviews.begin(); it != subviews.end(); it++)
		if ((*it)->sendEvent(event))
			return true;
	
	//We weren't lucky
	return Engine::Responder::sendEventToNextResponders(event);
}
