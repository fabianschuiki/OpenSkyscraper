#include "background.h"

#include "tower.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

TowerBackground::TowerBackground(Tower * tower) : Engine::Object(), tower(tower),
updateSkyTexturesIfNeeded(this, &TowerBackground::updateSkyTextures),
updateGroundTexturesIfNeeded(this, &TowerBackground::updateGroundTextures)
{
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

double TowerBackground::getRainAnimation()
{
	return rainAnimation;
}

void TowerBackground::setRainAnimation(double animation)
{
	if (rainAnimation != animation) {
		rainAnimation = animation;
		setRainAnimationFrame(rainAnimation * 2);
	}
}

void TowerBackground::advance(double dt)
{
	Engine::Object::advance(dt);
	
	//Advance the rain animation
	if (getSkyState() == Rain)
		setRainAnimation(fmod(getRainAnimation() + dt, 1));
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

void TowerBackground::update()
{
	updateSky();
	updateGroundTexturesIfNeeded();
}

void TowerBackground::updateSky()
{
	double time = tower->time;
	
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
	//Decide what variants of the sky should be interpolated
	std::string currentVariant = "";
	std::string targetVariant = "";
	
	switch (skyState) {
		case Night: {
			currentVariant = "night";
		} break;
			
		case EarlyDawn: {
			currentVariant = "night";
			targetVariant = "twilight";
		} break;
		case LateDawn: {
			currentVariant = "twilight";
			targetVariant = "day";
		} break;
			
		case Day: {
			currentVariant = "day";
		} break;
			
		case Worsening: {
			currentVariant = "day";
			targetVariant = "overcast";
		} break;
		case Rain: {
			std::stringstream n;
			n << "rain/" << getRainAnimationFrame();
			currentVariant = n.str();
		} break;
		case Improving: {
			currentVariant = "overcast";
			targetVariant = "day";
		} break;
			
		case EarlyDusk: {
			currentVariant = "day";
			targetVariant = "twilight";
		} break;
		case LateDusk: {
			currentVariant = "twilight";
			targetVariant = "night";
		} break;
	}
	
	//Load the required textures
	for (unsigned int i = 0; i < 10; i++) {
		
		//Current
		if (currentVariant.size())
			currentSkyTextures[i] = Engine::Texture::named(getSkyTextureName(i, currentVariant));
		else
			currentSkyTextures[i] = NULL;
		
		//Target
		if (targetVariant.size())
			targetSkyTextures[i] = Engine::Texture::named(getSkyTextureName(i, targetVariant));
		else
			targetSkyTextures[i] = NULL;
	}
}

void TowerBackground::updateGroundTextures()
{
	cityTexture = Engine::Texture::named(getCityTextureName());
	groundTexture = Engine::Texture::named(getGroundTextureName());
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void TowerBackground::draw(rectd dirtyRect)
{
	drawSky(dirtyRect);
	drawGround(dirtyRect);
}

void TowerBackground::drawSky(rectd dirtyRect)
{
	//Find the lowest and hightest sky indices to draw
	unsigned int lowest = clampi(floor(dirtyRect.minY() / 360), 0, 9);
	unsigned int highest = clampi(ceil(dirtyRect.maxY() / 360), 0, 9);
	
	//Create the interpolated texture quad and find the basic rectangle of one sky slice
	Engine::InterpolatedTextureQuad quad;
	quad.rect = dirtyRect;
	quad.rect.size.y = 360;	
	
	//Draw the slices
	for (unsigned int y = lowest; y <= highest; y++) {
		quad.rect.origin.y = y * quad.rect.size.y;
		quad.state0.texture = currentSkyTextures[y];
		quad.state1.texture = targetSkyTextures[y];
		quad.autogenerateTextureRect(true, false);
		quad.draw();
	}
}

void TowerBackground::drawGround(rectd dirtyRect)
{
	//Check whether the ground is visible
	if (dirtyRect.minY() < 0) {
		
		//Create the textured quad and find its rectangle
		Engine::TexturedQuad quad;
		quad.rect = dirtyRect;
		quad.rect.size.y = 360;
		quad.rect.origin.y = -quad.rect.size.y;
		
		//Set the texture, generate the texture rect and draw the quad
		quad.texture = groundTexture;
		quad.autogenerateTextureRect(true, false);
		quad.draw();
	}
	
	//Check whether the city is visible
	if (dirtyRect.minY() < 55 && dirtyRect.maxY() > 0) {
		
		//Create the textured quad for the city
		Engine::TexturedQuad quad;
		quad.rect = dirtyRect;
		quad.rect.size.y = cityTexture->size.y;
		quad.rect.origin.y = 0;
		
		//Set the texture, generate the coordinates and draw
		quad.texture = cityTexture;
		quad.autogenerateTextureRect(true, false);
		quad.draw();
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

TowerBackground::SkyState TowerBackground::getSkyState()
{
	return skyState;
}

void TowerBackground::setSkyState(SkyState state)
{
	if (skyState != state) {
		skyState = state;
		updateSkyTexturesIfNeeded.setNeeded();
	}
}

void TowerBackground::setSky(SkyState state, double interpolation)
{
	setSkyState(state);
	setSkyInterpolation(interpolation);
}

unsigned int TowerBackground::getRainAnimationFrame()
{
	return rainAnimationFrame;
}

void TowerBackground::setRainAnimationFrame(unsigned int frame)
{
	if (rainAnimationFrame != frame) {
		rainAnimationFrame = frame;
		updateSkyTexturesIfNeeded.setNeeded();
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Ground
//----------------------------------------------------------------------------------------------------

std::string TowerBackground::getCityTextureName()
{
	return "simtower/background/city";
}

std::string TowerBackground::getGroundTextureName()
{
	return "simtower/background/ground";
}
