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
	//Since we also want to be able to use ManagedMemory classes allocated statically, we have to
	//reduce the retain count by one here so that constructing and then destructing the instance re-
	//sults in a retain count of 0.
	retainCount--;
	assert(retainCount == 0);
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
	
	//If the retain count is one, this release yields a deconstruction. Note that we don't have to
	//reduce the retain count to 0 since this is done in the destructor for static instantiation
	//compatibility.
	if (retainCount == 1) {
		delete this;
		return NULL;
	}
	
	retainCount--;
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
