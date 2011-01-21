#ifndef OSS_CLASSIC_ITEMS_FACILITIES_OCCUPIABLE_H
#define OSS_CLASSIC_ITEMS_FACILITIES_OCCUPIABLE_H

#include "facility.h"


namespace OSS {
	namespace Classic {
		class OccupiableItem : public FacilityItem {
			
			/**
			 * Initialization
			 */
		public:
			OccupiableItem(Tower * tower, ItemDescriptor * descriptor);
			
			
			/**
			 * Occupancy
			 */
		private:
			bool occupied;
		public:
			bool isOccupied() const;
			void setOccupancy(bool o);
			
			virtual void willChangeOccupancy(bool newOccupancy) {}
			virtual void didChangeOccupancy() {}
			
			
			/**
			 * Simulation
			 */
		private:
			double occupyAt;
			
		public:
			double getOccupyAt();
			void setOccupyAt(double oa);
			
			virtual void advanceItem(double dt);
			virtual void advanceOccupancy(double dt);
			
			bool isSufficientlyAttractive();
			
			
			/**
			 * State
			 */
		public:
			virtual void update();
			virtual void updateOccupyAt() {}
			
			Updatable::Conditional<OccupiableItem> updateOccupyAtIfNeeded;
		};
	}
}


#endif
