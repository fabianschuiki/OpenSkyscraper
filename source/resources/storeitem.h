#ifndef OSS_STOREITEM_H
#define OSS_STOREITEM_H

#include "../general.h"
#include "../base/base.h"

#define OSSStoreItemStoreAccess(_itemclass_) \
static Store<_itemclass_> * store;\
static _itemclass_ * named(std::string name) { return store->itemNamed(name); }\
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
		std::string name;
		
		//Initialization
		StoreItem(std::string name) : name(name) {}
		
		/**
		 * In order to automatically unload a resource we override release() and queue the item for
		 * unloading when the retain count reaches 1.
		 */
		void release();
		
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
