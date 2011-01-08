#ifndef OSS_CLASSIC_ITEMS_FACILITIES_OCCUPIABLE_H
#define OSS_CLASSIC_ITEMS_FACILITIES_OCCUPIABLE_H

#include "../../external.h"

#include "facility.h"


namespace OSS {
	namespace Classic {
		class OccupiableItem : public FacilityItem {
		public:
			//Initialization
			OccupiableItem(Tower * tower, Item::Descriptor * descriptor);
			
			
			/**
			 * Occupation
			 */
		private:
			bool occupied;
		public:
			bool isOccupied() const;
			void setOccupied(bool occupied);
			virtual void onChangeOccupied();
			
			
			/**
			 * Occupancy Automation
			 */
		private:
			double autooccupyTime;
		public:
			double getAutooccupyTime();
			void setAutooccupyTime(double time);
			virtual void initAutooccupyTime();
			
			void resetOccupancyAutomation();
			void updateOccupancyAutomation();
			
			bool isSufficientlyAttractive();
			
			
			/**
			 * Simulation
			 */
			virtual void advance(double dt);
		};
	}
}


#endif
