#ifndef OSS_ENGINE_RESOURCES_SOUND_H
#define OSS_ENGINE_RESOURCES_SOUND_H

#include "../external.h"
#include "storeitem.h"

#include "store.h"

namespace OSS {
	namespace Engine {
		class Sound : public StoreItem {
		public:		
			//Initialization
			Sound(string name);
			~Sound();
			virtual string instanceName();
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
}


#endif
