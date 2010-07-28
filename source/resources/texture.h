#ifndef OSS_TEXTURE_H
#define OSS_TEXTURE_H

#include "../general.h"
#include "store.h"
#include "storeitem.h"


namespace OSS {
	class Texture : public StoreItem {
	public:
		OSSStoreItemStoreAccess(Texture)
		
		//Loading
		void load();
		void finalize();
		
		//Unloading
		void unfinalize();
		void unload();
	};
	
	typedef Store<Texture> TextureStore;
}


#endif
