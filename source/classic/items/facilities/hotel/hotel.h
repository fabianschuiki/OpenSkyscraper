#ifndef OSS_CLASSIC_ITEMS_FACILITIES_HOTEL_HOTEL_H
#define OSS_CLASSIC_ITEMS_FACILITIES_HOTEL_HOTEL_H

#include "../../../external.h"
#include "../occupiable.h"
#include "../../people/individuals/hotelguest.h"
#include "../../people/individuals/janitor.h"


namespace OSS {	
	namespace Classic {
		class HotelItem : public OccupiableItem {
		public:
			//Initialization
			HotelItem(Tower * tower, Item::Descriptor * descriptor);
			virtual std::string getTypeName() = 0;
			
			
			/**
			 * State
			 */
		public:
			typedef enum {
				kEmptyState,
				kOccupiedState,
				kAsleepState,
				kDirtyState,
				kInfestedState
			} State;
		private:
			State state;
		public:
			State getState() const;
			void setState(State state);
			virtual void onChangeState();
			
			
			/**
			 * Basic Sprites
			 */		
			std::string getTextureBaseName();
			unsigned int getTextureSliceIndex();
			virtual void updateBackground();
			
			
			/**
			 * Simulation
			 */
			void advance(double dt);
			void advanceGuests(double dt);
			
			
			/**
			 * Drawing
			 */
			void draw(rectd visibleRect);
			void drawGuests(rectd visibleRect);
			
			
			/**
			 * Guests
			 */
		private:
			typedef std::set< Pointer<HotelGuest> > Guests;
			Guests guests;
		public:
			virtual unsigned int getMaxNumberOfGuests();
			void initGuests();
			void updateGuests();
			void clearGuests();
			
			void addPerson(Person * person);
			void removePerson(Person * person);
			
			//Convenience
			bool areAllGuestsAsleep();
			
			
			/**
			 * Housekeeping
			 */
		private:
			Pointer<Janitor> assignedJanitor;
		public:
			Janitor * getAssignedJanitor();
			void setAssignedJanitor(Janitor * janitor);
			bool hasAssignedJanitor();
			
			
			/**
			 * Notifications
			 */
			void initAutooccupyTime();
			void onChangeOccupied();
			void onChangeGuestAsleep();
		};
	}
}


#endif
