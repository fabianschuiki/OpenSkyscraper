#include "storeitem.h"
#include "resources.h"

using namespace OSS;


void StoreItem::release() {
	Object::release();
	if (retainCount == 1)
		getStore()->queueItemForUnloading(this);
}
