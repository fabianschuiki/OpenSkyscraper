#include "funds.h"

#include "tower.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

TowerFunds::TowerFunds(Tower * tower) : Engine::Object(), tower(tower)
{
	funds = 2e6;
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
		//TODO: play the funds sound
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
