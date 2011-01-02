#ifndef OSS_SIMTOWER_H
#define OSS_SIMTOWER_H

#include "../general.h"
#include "../base/base.h"


namespace OSS {
	class SimTower : public Singleton<SimTower, Object> {
	private:
		//Resources
		class Resource : public Object {
		public:
			unsigned short id;
			unsigned short type;
			unsigned int length;
			void * data;
			std::string getName();
			std::string getDumpName();
		};
		
		typedef std::vector< Pointer<Resource> > ResourceVector;
		ResourceVector resources;
		Resource * getResource(unsigned short type, unsigned short id);
		
		//Resource Types
		enum {
			kBitmapResource			= 0x2,
			kMenuResource			= 0x4,
			kDialogBoxResource		= 0x5,
			kStringTableResource	= 0x6,
			kSoundResource			= 0x7F0A
		};
		
		//Resource Names
		typedef struct {
			unsigned short resourceID;
			std::string name;
		} ResourceName;
		static ResourceName bitmapResourceNames[];
		static ResourceName soundResourceNames[];
		static std::string getNameForResource(Resource * resource);
		
	public:
		//Loads the resources of the SimTower executable
		void loadResources();
		
		//Extracts various components from the loaded resources
		void extractAll();
		void extractTextures();
		void extractSounds();
		
		//Creates one or more textures from the given information
		void postprocessTexture(std::string textureName, const void * buffer, unsigned int bufferLength);
		void applyReplacementPalette(unsigned short id);
		void spawnSkyTextures(std::string textureName, ILuint image);
	};
}


#endif
