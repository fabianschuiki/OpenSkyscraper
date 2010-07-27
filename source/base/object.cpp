#include "object.h"

using namespace OSS;


//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

Object::Object()
{
	retainCount = 0;
}

Object::~Object()
{
	assert(retainCount == 0);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Memory Management
//----------------------------------------------------------------------------------------------------

void Object::retain()
{
	retainCount++;
}

void Object::release()
{
	assert(retainCount > 0);
	retainCount--;
	if (retainCount <= 0)
		delete this;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Runtime Type Information
//----------------------------------------------------------------------------------------------------

/**
 * Returns the object's class name. This function may be realized in various ways,
 * yet using typeid makes the whole thing portable among many platforms.
 */
std::string Object::className()
{
	return typeid(*this).name();
}

/**
 * Returns the object's instance name. The instance name itself doesn't fit anywhere
 * into the class/instance model directly, yet it provides a convenient way to name
 * your instances. Other description functions should use instanceName() whenever
 * they want to refer to the object.
 *
 * Hence complicated or frequently used objects may be given names like George, Fred,
 * or Bill, which are easier to mentally refer to while debugging than hexadecimal
 * memory locations.
 *
 * The default implementation returns "className this".
 */
std::string Object::instanceName()
{
	char str[64];
	sprintf(str, "%s %p", this->className().c_str(), this);
	return str;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Description
//----------------------------------------------------------------------------------------------------

/**
 * Returns the object's description in human-readable form. This is a convenient
 * place to usefully describe your objects as a string. Whenever you need to log
 * a message related to an object, you may use description() on the object to
 * include useful information on the object in your log.
 *
 * The default implementation returns "<instanceName>". Note the triangle brackets
 * used to visually identify the description as an object. For some long
 * descriptions it may be hard to tell the description from the rest of the log
 * message without the brackets.
 */
std::string Object::description()
{
	std::string res = "<";
	res += this->instanceName();
	res += ">";
	return res;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Events
//----------------------------------------------------------------------------------------------------

bool Object::handleEvent(Event * event)
{
	switch (event->type) {
		case kEventSDL: return eventSDL(event->sdl.event); break;
	}
	return false;
}
