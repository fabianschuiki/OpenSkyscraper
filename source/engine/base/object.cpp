#include "object.h"

#include <cstdio>

using namespace OSS;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Class Inheritance
//----------------------------------------------------------------------------------------------------

Object::Class Object::getClass()
{
	return (Class)typeid(*this);
}

bool Object::isMemberOfClass(Class c)
{
	return (typeid(*this) == c);
}

bool Object::isKindOfClass(Class c)
{
	return isMemberOfClass(c);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Description
//----------------------------------------------------------------------------------------------------

/**
 * Returns the object's class name. This function may be realized in various ways,
 * yet using typeid makes the whole thing portable among many platforms.
 */
string Object::className() const
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
string Object::instanceName() const
{
	char str[128];
	snprintf(str, 128, "%s %p", this->className().c_str(), this);
	return str;
}

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
string Object::description() const
{
	string res = "<";
	res += this->instanceName();
	res += ">";
	return res;
}
