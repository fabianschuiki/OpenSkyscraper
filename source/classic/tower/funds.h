#ifndef OSS_CLASSIC_TOWER_FUNDS_H
#define OSS_CLASSIC_TOWER_FUNDS_H

#include "../external.h"


namespace OSS {
	namespace Classic {
		class Tower;
		
		class TowerFunds : public Engine::Object {
			
			/**
			 * Cosntruction
			 */
		public:
			const Pointer<Tower> tower;
			
			TowerFunds(Tower * tower);
			
			
			/**
			 * Funds
			 */
		private:
			long funds;
			
		public:
			long getFunds();
			void setFunds(long f);
			
			void transfer(long amount);
			bool hasSufficient(long requestedAmount);
		};
	}
}


#endif
