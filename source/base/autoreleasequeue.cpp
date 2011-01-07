#include "autoreleasequeue.h"

#include "managedmemory.h"

using namespace OSS;
using namespace Base;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Nesting
//----------------------------------------------------------------------------------------------------

std::stack<AutoreleaseQueue *> AutoreleaseQueue::stack;

AutoreleaseQueue::AutoreleaseQueue()
{
	stack.push(this);
}

AutoreleaseQueue::~AutoreleaseQueue()
{
	assert(stack.top() == this);
	drain();
	stack.pop();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Autoreleasing
//----------------------------------------------------------------------------------------------------

void AutoreleaseQueue::addGarbage(ManagedMemory * garbage)
{
	garbageQueue.push(garbage);
}

void AutoreleaseQueue::autorelease(ManagedMemory * garbage)
{
	assert(!stack.empty() && "autorelease without a queue in place; memory leaking!");
	stack.top()->addGarbage(garbage);
}

void AutoreleaseQueue::drain()
{
	while (!garbageQueue.empty()) {
		garbageQueue.front()->release();
		garbageQueue.pop();
	}
}
