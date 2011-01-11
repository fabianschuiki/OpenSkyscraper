#include "managedmemory.h"

#include "autoreleasequeue.h"

using namespace OSS;
using namespace Base;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

ManagedMemory::ManagedMemory()
{
	retainCount = 1;
}

ManagedMemory::~ManagedMemory()
{
	//assert(retainCount == 0);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Reference Counting
//----------------------------------------------------------------------------------------------------

ManagedMemory * ManagedMemory::retain()
{
	retainCount++;
	return this;
}

ManagedMemory * ManagedMemory::release()
{
	assert(retainCount > 0 && "trying to release instance already released");
	
	if (--retainCount == 0) {
		delete this;
		return NULL;
	}
	
	return this;
}

ManagedMemory * ManagedMemory::autoretain()
{
	retain();
	autorelease();
	return this;
}

ManagedMemory * ManagedMemory::autorelease()
{
	AutoreleaseQueue::autorelease(this);
	return this;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Memory Allocation
//----------------------------------------------------------------------------------------------------

void * ManagedMemory::operator new(std::size_t size)
{
	void * memory = malloc(size);
	assert(memory);
	bzero(memory, size);
	return memory;
}

void ManagedMemory::operator delete(void * ptr)
{
	free(ptr);
}
