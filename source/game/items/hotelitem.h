#ifndef OSS_HOTELITEM_H
#define OSS_HOTELITEM_H


#include "../../general.h"
#include "../occupiableitem.h"
#include "../people/hotelguest.h"


namespace OSS {
	class HotelItem : public OccupiableItem {
	public:
		//Initialization
		HotelItem(Tower * tower, Item::Descriptor * descriptor);
		virtual std::string getTypeName() = 0;
		
		
		/**
		 * State
		 */
	private:
		typedef enum {
			kEmptyState,
			kOccupiedState,
			kAsleepState,
			kDirtyState,
			kInfestedState
		} State;
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
		virtual void advance(double dt);
		
		
		/**
		 * Guests
		 */
	private:
		typedef std::set< Pointer<HotelGuest> > Guests;
		Guests guests;
	public:
		void initGuests();
		void updateGuests();
		void clearGuests();
		void removePerson(Person * person);
		
		//Convenience
		bool areAllGuestsAsleep();
		
		
		/**
		 * Notifications
		 */
		void initAutooccupyTime();
		void onChangeOccupied();
		void onChangeGuestAsleep();
	};
}


#endif
