#ifndef OSS_CLASSIC_ITEMS_FACILITIES_OFFICE_OFFICE_H
#define OSS_CLASSIC_ITEMS_FACILITIES_OFFICE_OFFICE_H

#include "../occupiable.h"


namespace OSS {
	namespace Classic {
		class OfficeWorker;
		
		class OfficeItem : public OccupiableItem {
		public:
			static ItemDescriptor descriptor;
			
			//Initialization
			OfficeItem(Tower * tower);
			virtual string getTypeName() const { return "office"; }
			
			
			//Workers
		private:
			typedef std::set< Pointer<OfficeWorker> > Workers;
			Workers workers;
			
			virtual void didChangeOccupancy();
			
			
			//Simulation
		public:
			//void advanceItem(double dt);
			virtual bool shouldOccupy();
			
			
			//State
		public:
			virtual void updateBackground();
			virtual void updateOccupyAt();
			
			
			//Drawing
		public:
			string getTextureBaseName();
			unsigned int getTextureSliceIndex();
		};
	}
}


#include "../../../people/individuals/officeworker.h"


#endif
