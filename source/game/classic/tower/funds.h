#ifndef OSS_GAME_CLASSIC_TOWER_FUNDS_H
#define OSS_GAME_CLASSIC_TOWER_FUNDS_H

#include "tower.h"


namespace OSS {
	namespace Classic {		
		class TowerFunds : public GameObject {
			
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
			Pointer<SoundEffect> transferSound;
			
		public:
			long getFunds();
			void setFunds(long f);
			
			void transfer(long amount);
			bool hasSufficient(long requestedAmount);
		};
	}
}


#endif
