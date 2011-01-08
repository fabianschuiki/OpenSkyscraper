#include "background.h"

#include "tower.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

TowerBackground::TowerBackground(Tower * tower) : Core::Object(), tower(tower)
{
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

void TowerBackground::update()
{
	updateSky();
}

void TowerBackground::updateSky()
{
	double time = tower->time();
	
	//Night
	if (time < 5 || time >= 19)
		setSky(Night, 0);
	
	//Dawn
	else if (time >= 5 && time < 6)
		setSky(EarlyDawn, (time - 5));
	else if (time >= 6 && time < 7)
		setSky(LateDawn, (time - 6));
	
	//Dusk
	else if (time >= 17 && time < 18)
		setSky(EarlyDusk, (time - 17));
	else if (time >= 18 && time < 19)
		setSky(LateDusk, (time - 18));
	
	//Day
	else if (!tower->isRainyDay)
		setSky(Day, 0);
	
	//Rain
	else if (time >= 7 && time < 8)
		setSky(Worsening, (time - 7));
	else if (time >= 16 && time < 17)
		setSky(Improving, (time - 16));
	else
		setSky(Rain, 0);
	
	//Update the textures if required
	updateSkyTexturesIfNeeded();
}

void TowerBackground::updateSkyTextures()
{
	std::string currentVariant = "";
	std::string targetVariant = "";
	
	switch (skyState) {
		case Night:
			currentVariant = "night";
			break;
			
		case EarlyDawn:
			currentVariant = "night";
			targetVariant = "twilight";
			break;
		case LateDawn:
			currentVariant = "twilight";
			targetVariant = "day";
			break;
	}
	
	for (unsigned int i = 0; i < 10; i++) {
		if (currentVariant.size())
			currentSkyTextures[i] = Texture::named(getSkyTextureName(i, currentVariant));
		if (targetVariant.size())
			targetSkyTextures[i] = Texture::named(getSkyTextureName(i, targetVariant));
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Sky
//----------------------------------------------------------------------------------------------------

std::string TowerBackground::getSkyTextureName(unsigned int index, std::string variant)
{
	std::stringstream name;
	name << "simtower/background/sky/";
	name << index << "/";
	name << variant;
	return name.str();
}

double TowerBackground::getSkyInterpolation()
{
	return skyInterpolation;
}

void TowerBackground::setSkyInterpolation(double f)
{
	skyInterpolation = f;
}

SkyState TowerBackground::getSkyState()
{
	return skyState;
}

void TowerBackground::setSkyState(SkyState state)
{
	if (skyState != state) {
		skyState = state;
		setNeedsUpdateSkyTextures();
	}
}

void TowerBackground::setSky(SkyState state, double interpolation)
{
	setSkyState(state);
	setSkyInterpolation(interpolation);
}
