#include "funds.h"

#include "tower.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

TowerFunds::TowerFunds(Tower * tower) : Responder(), tower(tower)
{
	//Set the initial money to 2'000'000
	funds = 2e6;
	
	//Initialize the funds transfer sound effect.
	transferSound = new Engine::SoundEffect();
	transferSound->sound = Engine::Sound::named("simtower/cash");
	transferSound->layer = Engine::SoundEffect::kTopLayer;
	transferSound->minIntervalBetweenPlaybacks = 0.1;
	transferSound->loopCount = 2;
	transferSound->copyBeforeUse = true;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Funds
//----------------------------------------------------------------------------------------------------

long TowerFunds::getFunds()
{
	return funds;
}

void TowerFunds::setFunds(long f)
{
	if (funds != f) {
		funds = f;
		
		//Play the funds transfer sound effect. Sounds like an old cashier.
		Engine::Audio::getCurrent()->play(transferSound);
	}
}

void TowerFunds::transfer(long amount)
{
	setFunds(getFunds() + amount);
}

bool TowerFunds::hasSufficient(long requestedAmount)
{
	return (getFunds() >= requestedAmount);
}
