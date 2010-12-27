#ifndef OSS_STORE_H
#define OSS_STORE_H

#include "../general.h"
#include "../base/base.h"
#include "storeitem.h"


namespace OSS {
	class AbstractStore : public Object {
	public:		
		virtual bool loadNext() = 0;
		virtual bool finalizeNext() = 0;
		
		virtual bool unfinalizeNext() = 0;
		virtual bool unloadNext() = 0;
		
		virtual void queueItemForLoading(StoreItem * item) = 0;
		virtual void queueItemForUnloading(StoreItem * item) = 0;
		
		//Adds a loaded item to the store's map and queues it for loading
		virtual void registerItem(StoreItem * item) = 0;
	};
	
	template <class T> class Store : public AbstractStore {
	public:		
		//Initialization
		Store() {
			assert(T::store == NULL);
			T::store = this;
			OSSObjectLog << "initialized" << std::endl;
		}
		~Store() {
			itemsToLoad.clear();
			itemsToFinalize.clear();
			
			itemsToUnfinalize.clear();
			itemsToUnload.clear();
			
			items.clear();
		}
		
		
		/**
		 * Items
		 */
		
	public:
		T * itemNamed(std::string itemName) {
			//Return the existing item if possible
			T * item = items[itemName];
			if (item)
				return item;
			
			//If the item is not in the map, create one
			return new T(itemName);
		}
		
		void registerItem(StoreItem * item) {
			items[item->name] = (T*)item;
			queueItemForLoading(item);
		}
		
	private:
		std::map< std::string, Pointer<T> > items;
		
		
		/**
		 * Loading
		 */
		
	public:
		//Working the load queues
		bool loadNext() {
			if (itemsToLoad.empty()) return false;
			T * item = itemsToLoad.front();
			OSSObjectLog << item->description() << std::endl;
			item->load();
			itemsToFinalize.push_back(item);
			itemsToLoad.pop_front();
			return true;
		}
		bool finalizeNext() {
			if (itemsToFinalize.empty()) return false;
			T * item = itemsToFinalize.front();
			OSSObjectLog << item->description() << std::endl;
			item->finalize();
			itemsToFinalize.pop_front();
			return true;
		}
		
		//Working the unload queues
		bool unfinalizeNext() {
			if (itemsToUnfinalize.empty()) return false;
			T * item = itemsToUnfinalize.front();
			OSSObjectLog << item->description() << std::endl;
			item->unfinalize();
			itemsToUnload.push_back(item);
			itemsToUnfinalize.pop_front();
			return true;
		}
		bool unloadNext() {
			if (itemsToUnload.empty()) return false;
			T * item = itemsToUnload.front();
			OSSObjectLog << item->description() << std::endl;
			item->unload();
			itemsToUnload.pop_front();
			return true;
		}
		
	protected:
		void queueItemForLoading(StoreItem * item) {
			OSSObjectLog << item->description() << std::endl;
			itemsToLoad.push_back((T*)item);
		}
		void queueItemForUnloading(StoreItem * item) {
			OSSObjectLog << item->description() << std::endl;
			itemsToUnload.push_back((T*)item);
		}
		
	private:
		//Loading
		std::deque< Pointer<T> > itemsToLoad;
		std::deque< Pointer<T> > itemsToFinalize;
		
		//Unloading
		std::deque< Pointer<T> > itemsToUnfinalize;
		std::deque< Pointer<T> > itemsToUnload;
	};
}


#endif
