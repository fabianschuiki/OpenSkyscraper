#include "object~.h"

using namespace OSS;
using namespace Base;





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
