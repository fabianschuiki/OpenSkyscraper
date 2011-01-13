#ifndef OSS_ENGINE_RESOURCES_STORE_H
#define OSS_ENGINE_RESOURCES_STORE_H

#include "../external.h"
#include "../base/object.h"

#include "../base/pointer.h"
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
		virtual void queueAllItemsForRefinalizing() = 0;
	};
	
	template <typename T> class Store : public AbstractStore {
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
		T * itemNamed(string itemName) {
			//Return the existing item if possible
			T * item = items[itemName];
			if (item)
				return item;
			
			//If the item is not in the map, create one and store in the items map
			item = new T(itemName);
			items[itemName] = item;
			
			//Queue the item for loading and return it
			queueItemForLoading(item);
			return item;
		}
		
	private:
		map< string, Pointer<T> > items;
		
		
		/**
		 * Loading
		 */
		
	public:
		//Working the load queues
		bool loadNext() {
			if (itemsToLoad.empty()) return false;
			T * item = itemsToLoad.front();
			//OSSObjectLog << item->description() << std::endl;
			item->load();
			//itemsToFinalize.push_back(item);
			itemsToLoad.pop_front();
			loadedItems.insert(item);
			return true;
		}
		bool finalizeNext() {
			if (itemsToFinalize.empty()) return false;
			T * item = itemsToFinalize.front();
			//OSSObjectLog << item->description() << std::endl;
			item->finalize();
			itemsToFinalize.pop_front();
			finalizedItems.insert(item);
			return true;
		}
		
		//Working the unload queues
		bool unfinalizeNext() {
			if (itemsToUnfinalize.empty()) return false;
			T * item = itemsToUnfinalize.front();
			//OSSObjectLog << item->description() << std::endl;
			item->unfinalize();
			//itemsToUnload.push_back(item);
			itemsToUnfinalize.pop_front();
			finalizedItems.erase(item);
			return true;
		}
		bool unloadNext() {
			if (itemsToUnload.empty()) return false;
			T * item = itemsToUnload.front();
			//OSSObjectLog << item->description() << std::endl;
			item->unload();
			itemsToUnload.pop_front();
			loadedItems.erase(item);
			return true;
		}
		
	public:
		void queueItemForLoading(StoreItem * item) {
			//OSSObjectLog << item->description() << std::endl;
			itemsToLoad.push_back((T*)item);
			itemsToFinalize.push_back((T*)item);
		}
		void queueItemForUnloading(StoreItem * item) {
			//OSSObjectLog << item->description() << std::endl;
			itemsToUnfinalize.push_back((T*)item);
			itemsToUnload.push_back((T*)item);
		}
		void queueAllItemsForRefinalizing() {
			OSSObjectLog << std::endl;
			typename std::set< Pointer<T> >::iterator item;
			for (item = finalizedItems.begin(); item != finalizedItems.end(); item++) {
				itemsToUnfinalize.push_back(*item);
				itemsToFinalize.push_back(*item);
			}
		}
		
	private:
		//Loading
		std::deque< Pointer<T> > itemsToLoad;
		std::deque< Pointer<T> > itemsToFinalize;
		
		//Unloading
		std::deque< Pointer<T> > itemsToUnfinalize;
		std::deque< Pointer<T> > itemsToUnload;
		
		//Sets
		std::set< Pointer<T> > loadedItems;
		std::set< Pointer<T> > finalizedItems;
	};
}


#endif
