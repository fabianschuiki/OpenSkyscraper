#ifndef OSS_SOUND_H
#define OSS_SOUND_H

#include "../general.h"
#include "store.h"
#include "storeitem.h"


namespace OSS {
	class Sound : public StoreItem {
	public:		
		//Initialization
		Sound(std::string name);
		~Sound();
		virtual std::string instanceName();
		void assignLoadedData(const void * data, ALuint length);
		
		//Store
		OSSStoreItemStoreAccess(Sound)
		
		//Loading
		void load();
		void finalize();
		
		//Unloading
		void unfinalize();
		void unload();
		
		//OpenAL Buffer
		ALuint bufferID;
	};
	
	typedef Store<Sound> SoundStore;
}


#endif
