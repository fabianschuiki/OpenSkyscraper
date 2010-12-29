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
		
		//Resource Names
		typedef struct {
			unsigned short resourceID;
			std::string name;
		} ResourceName;
		static ResourceName resourceNames[];
		static std::string getNameForResource(Resource * resource);
		
	public:
		//Loads the resources of the SimTower executable
		void loadResources();
		
		//Extracts various components from the loaded resources
		void extractAll();
		void extractTextures();
		void extractSounds();
	};
}


#endif
