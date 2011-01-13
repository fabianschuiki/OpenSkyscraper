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
	autorelease();
}

ManagedMemory::~ManagedMemory()
{
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
	AutoreleaseQueue::autorelease(this);
}
