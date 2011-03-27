#include "managedmemory.h"

#include "autoreleasequeue.h"

using namespace OSS;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

ManagedMemory::ManagedMemory()
{
	retainCount = 1;
	//TODO: re-enable this as soon as the sprites are fixed
	autorelease();
}

ManagedMemory::~ManagedMemory()
{
	//TODO: re-enable this assertion as soon as the problem with statically allocated memory managed
	//objects (e.g. sprites) is solved.
	assert(retainCount == 0);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Reference Counting
//----------------------------------------------------------------------------------------------------

void ManagedMemory::retain()
{
	retainCount++;
}

void ManagedMemory::release()
{
	assert(retainCount > 0 && "trying to release instance already released");
	
	if (--retainCount == 0)
		delete this;
}

void ManagedMemory::autoretain()
{
	retain();
	autorelease();
}

void ManagedMemory::autorelease()
{
	if (typeid(*this).name() == std::string("N3OSS7Classic5RouteE"))
		std::cout << "blob!" << std::endl;
	AutoreleaseQueue::autorelease(this);
}
