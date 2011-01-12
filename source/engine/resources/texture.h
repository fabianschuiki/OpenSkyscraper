#ifndef OSS_ENGINE_RESOURCES_TEXTURE_H
#define OSS_ENGINE_RESOURCES_TEXTURE_H

#include "../external.h"
#include "storeitem.h"

#include "store.h"

namespace OSS {
	namespace Engine {
		class Texture : public StoreItem {
		public:		
			//Initialization
			Texture(string name);
			~Texture();
			virtual string instanceName();
			void assignLoadedData(ILenum type, const void * data, ILuint length);
			void assignLoadedImage(ILuint image);
			
			//Store
			OSSStoreItemStoreAccess(Texture)
			
			//Loading
			void load();
			void finalize();
			
			//Unloading
			void unfinalize();
			void unload();
			
			//Texture
			ILuint tempImage;
			GLuint textureID;
			int2 size;
			
			//Binding
			void bind();
			static void unbind();
			
			//Post-processing
			color3d transparentColor;
			vector<color3d> transparentColors;
			bool useTransparentColor;
		};
		
		typedef Store<Texture> TextureStore;
	}
}


#endif
