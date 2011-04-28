#ifndef OSS_CLASSIC_ITEMS_FACILITIES_ENTERTAINMENT_H
#define OSS_CLASSIC_ITEMS_FACILITIES_ENTERTAINMENT_H

#include "../facility.h"


namespace OSS {
	namespace Classic {
		class EntertainmentItem : public FacilityItem {
			
			/**
			 * Initialization
			 */
		public:
			EntertainmentItem(Tower * tower, ItemDescriptor * descriptor);
			
			
			/**
			 * Openness
			 */
		private:
			bool open;
			
		public:
			bool isOpen() const;
			void setOpen(bool o);
			virtual bool shouldBeOpen();
			
			virtual void willChangeOpenness(bool newOpen) {}
			virtual void didChangeOpenness() {}
			
			
			/**
			 * Simulation
			 */
		public:
			virtual void advanceItem(double dt);
			virtual void advanceOpenness(double dt);
		};
	}
}


#endif

