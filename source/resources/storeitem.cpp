#include "storeitem.h"
#include "resources.h"

using namespace OSS;


void StoreItem::release() {
	Object::release();
	//disabled the following unloading since it led tu some strange behaviour and bugs
	/*if (retainCount == 1)
		getStore()->queueItemForUnloading(this);*/
}
