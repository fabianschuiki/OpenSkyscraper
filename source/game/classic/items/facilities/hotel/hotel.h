#ifndef OSS_CLASSIC_ITEMS_FACILITIES_HOTEL_HOTEL_H
#define OSS_CLASSIC_ITEMS_FACILITIES_HOTEL_HOTEL_H

#include "../occupiable.h"


namespace OSS {	
	namespace Classic {
		class HotelGuest;
		class Janitor;
		
		class HotelItem : public OccupiableItem {
			
			/**
			 * Initialization
			 */
		public:
			HotelItem(Tower * tower, ItemDescriptor * descriptor);
			
			
			/**
			 * Guests
			 */
		private:
			typedef std::set< Pointer<HotelGuest> > Guests;
			Guests guests;
			
		public:
			virtual unsigned int getMaxNumberOfGuests();
			
			bool areAllGuestsAsleep();
			bool haveAllGuestsCheckedOut();
			
			virtual void didChangeOccupancy();
			
			
			/**
			 * Janitor
			 */
		private:
			Pointer<Janitor> assignedJanitor;
			
		public:
			Janitor * getAssignedJanitor();
			void setAssignedJanitor(Janitor * janitor);
			bool hasAssignedJanitor();
			
			
			/**
			 * People
			 */
		public:
			virtual void didAddPerson(Person * person);
			virtual void willRemovePerson(Person * person);
			
			
			/**
			 * Simulation
			 */
		public:
			void advanceItem(double dt);
			
			
			/**
			 * State
			 */
		private:
			bool asleep;
			bool dirty;
			bool infested;
			
		public:
			bool isAsleep();
			void setAsleep(bool a);
			
			bool isDirty();
			void setDirty(bool d);
			
			bool isInfested();
			void setInfested(bool i);
			
			virtual void updateItem();
			virtual void updateBackground();
			
			
			/**
			 * Drawing
			 */
		public:
			string getTextureBaseName();
			unsigned int getTextureSliceIndex();
		};
	}
}


#include "../../../people/individuals/hotelguest.h"
#include "../../../people/individuals/janitor.h"


#endif
