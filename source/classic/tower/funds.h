#ifndef OSS_CLASSIC_TOWER_FUNDS_H
#define OSS_CLASSIC_TOWER_FUNDS_H

#include "../external.h"
#include "../responder.h"


namespace OSS {
	namespace Classic {
		class Tower;
		
		class TowerFunds : public Responder {
			
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
			Pointer<Engine::SoundEffect> transferSound;
			
		public:
			long getFunds();
			void setFunds(long f);
			
			void transfer(long amount);
			bool hasSufficient(long requestedAmount);
		};
	}
}


#endif
