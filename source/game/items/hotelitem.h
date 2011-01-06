#ifndef OSS_HOTELITEM_H
#define OSS_HOTELITEM_H


#include "../../general.h"
#include "../occupiableitem.h"
#include "../people/hotelguest.h"
#include "../people/janitor.h"


namespace OSS {	
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


#endif
