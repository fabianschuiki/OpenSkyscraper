#ifndef OSS_HOTELITEM_H
#define OSS_HOTELITEM_H


#include "../../general.h"
#include "../facilityitem.h"


namespace OSS {
	class HotelItem : public FacilityItem {
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
		
		double tc;
		virtual void advance(double dt);
	};
}


#endif
