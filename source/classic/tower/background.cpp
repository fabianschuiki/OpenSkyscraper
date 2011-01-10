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

bool TowerBackground::isRainyDay()
{
	return rainyDay;
}

void TowerBackground::setRainyDay(bool rainy)
{
	if (rainyDay != rainy) {
		rainyDay = rainy;
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
	double time = tower->time->getTime();
	
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
	else if (!isRainyDay())
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
			std::stringstream s;
			s << "rain/" << getRainAnimationFrame();
			currentVariant = s.str();
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
	
	//Load the required sky textures
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
	
	//Clouds don't have rain animation, so they're always showing overcast
	if (skyState == Rain)
		currentVariant = "overcast";
	
	//Load the required cloud textures
	for (unsigned int i = 0; i < 4; i++) {
		
		//Current
		if (currentVariant.size())
			currentCloudTextures[i] = Engine::Texture::named(getCloudTextureName(i, currentVariant));
		else
			currentCloudTextures[i] = NULL;
		
		//Target
		if (targetVariant.size())
			targetCloudTextures[i] = Engine::Texture::named(getCloudTextureName(i, targetVariant));
		else
			targetCloudTextures[i] = NULL;
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
	
	//Draw the clouds
	const int2 cloudGrid(250, 100);
	for (int x = dirtyRect.minX() / cloudGrid.x; x <= dirtyRect.maxX() / cloudGrid.x; x++) {
		for (int y = dirtyRect.minY() / cloudGrid.y; y <= dirtyRect.maxY() / cloudGrid.y; y++) {
			
			//Decide whether to draw a cloud here at all
			if (!hasCloudAt(int2(x, y)))
				continue;
			
			//Decide what cloud texture to use
			unsigned int textureIndex = (cloudNoise(x, y) + 1.0) * 2;
			
			Engine::Texture * texture0 = currentCloudTextures[textureIndex];
			Engine::Texture * texture1 = targetCloudTextures[textureIndex];
			if (!texture0 || !texture1)
				continue;
			
			//Draw the quad
			Engine::InterpolatedTextureQuad quad;
			quad.rect = rectd(double2(x, y) * cloudGrid, texture0->size);
			quad.rect.origin -= quad.rect.size / 2;
			quad.state0.texture = texture0;
			quad.state1.texture = texture1;
			quad.draw();
		}
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

std::string TowerBackground::getCloudTextureName(unsigned int index, std::string variant)
{
	std::stringstream name;
	name << "simtower/background/cloud/";
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

bool TowerBackground::hasCloudAt(double2 location)
{
	return (cloudNoise(location.x, location.y) > 0.75);
}

double TowerBackground::cloudNoise(double x, double y)
{
	int n=(int)x+(int)y*57;
	n=(n<<13)^n;
	int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
	return 1.0-((double)nn/1073741824.0);
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
