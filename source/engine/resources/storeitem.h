#ifndef OSS_ENGINE_RESOURCES_STOREITEM_H
#define OSS_ENGINE_RESOURCES_STOREITEM_H

#include "../external.h"
#include "../base/object.h"

#define OSSStoreItemStoreAccess(_itemclass_) \
static Store<_itemclass_> * store;\
static _itemclass_ * named(string name) { return store->itemNamed(name); }\
AbstractStore * getStore() { return _itemclass_::store; }


namespace OSS {
	class AbstractStore;
	
	class StoreItem : public Object {
	public:
		/**
		 * Your subclasses need to declare several store-related static variables and functions so
		 * that they are aware of their corresponding store. Use the OSSStoreItemStoreAccess() macro
		 * to auto-generate these accessors for your class.
		 */
		
		//The store this item is located in
		virtual AbstractStore * getStore() = 0;
		
		//Name of the item
		string name;
		
		//Initialization
		StoreItem(string name) : name(name) {}
		
		//State
		bool isLoaded;
		bool isFinalized;
		
		//Loading
		virtual void load() = 0;
		virtual void finalize() {}
		
		//Unloading
		virtual void unfinalize() {}
		virtual void unload() = 0;
	};
}


#endif
