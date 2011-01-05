#include "hotelitem.h"

using namespace OSS;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

HotelItem::HotelItem(Tower * tower, Item::Descriptor * descriptor) : FacilityItem(tower, descriptor)
{
	state = kEmptyState;
	tc = 0;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

HotelItem::State HotelItem::getState() const
{
	return state;
}

void HotelItem::setState(State state)
{
	if (this->state != state) {
		this->state = state;
		onChangeState();
	}
}

void HotelItem::onChangeState()
{
	updateBackground();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Basic Sprites
//----------------------------------------------------------------------------------------------------

std::string HotelItem::getTextureBaseName()
{
	std::stringstream s;
	s << "simtower/facilities/hotel/";
	s << getTypeName();
	s << "/";
	s << getVariant();
	return s.str();
}

unsigned int HotelItem::getTextureSliceIndex()
{
	//Choose the base index
	unsigned int index;
	switch (getState()) {
		case kOccupiedState:	index = 0; break;
		case kAsleepState:		index = 2; break;
		case kEmptyState:		index = 3; break;
		case kDirtyState:		index = 5; break;
		case kInfestedState:	index = 7; break;
	}
	
	//If the index doesn't point at the asleep slice and the room is not lit, select the next slice
	//which is the unlit version.
	if (index != 2 && !isLit())
		index++;
	
	return index;
}

void HotelItem::updateBackground()
{
	FacilityItem::updateBackground();
	
	//Determine what slice of the textures we're supposed to show
	unsigned int slice = getTextureSliceIndex();
	
	//The first slice is in its own texture for some reason
	if (slice == 0) {
		backgrounds[0].texture = Texture::named(getTextureBaseName() + "/0");
		backgrounds[0].textureRect = rectd(0, 0, 1, 1);
	} else {
		backgrounds[0].texture = Texture::named(getTextureBaseName() + "/1");
		backgrounds[0].textureRect = rectd((slice - 1) * 0.125, 0, 0.125, 1);
	}
}

void HotelItem::advance(double dt)
{
	FacilityItem::advance(dt);
	
	tc += dt;
	if (tc >= 3.0) {
		tc -= 3.0;
		
		int s = getState();
		if (s++ == kInfestedState)
			s = kEmptyState;
		setState((State)s);
	}
}
