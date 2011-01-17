#include "item.h"

#include "items.h"

using namespace OSS;
using namespace Classic;


static ItemDescriptor floorItemDescriptor = {
	kFloorType,
	kStructureGroup,
	kFacilityCategory,
	1,
	(kFlexibleWidthAttribute),
	500,
	int2(1, 1),
	int2(1, 1)
};





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

Item::Item(Tower * tower, ItemDescriptor * descriptor) :
tower(tower), descriptor(descriptor),
updateConstructionIfNeeded(this, &Item::updateConstruction, &updateIfNeeded),
updateItemIfNeeded(this, &Item::updateItem, &updateIfNeeded),
updateBackgroundIfNeeded(this, &Item::updateBackground, &updateItemIfNeeded)
{
	assert(tower && descriptor);
	
	//Reset all member values	
	constructed = true;
	constructionProgress = 0;
	
	unifiedBackground = false;
}

bool Item::isInTower()
{
	return tower->structure->containsItem(this);
}

Item * Item::make(Tower * tower, ItemDescriptor * descriptor, recti rect)
{
	if (!tower || !descriptor)
		return NULL;
	
	//Allocate the new item according to the descriptor type
	Item * instance = NULL;
	switch (descriptor->type) {
			//Structure
		case kLobbyType:			instance = new LobbyItem(tower); break;
		case kStairsType:			instance = new StairsItem(tower); break;
		case kEscalatorType:		instance = new EscalatorItem(tower); break;
			
			//Elevator
		/*case kStandardElevatorType:	instance = new StandardElevatorItem(tower); break;
			
			//Office
		case kOfficeType:			instance = new OfficeItem(tower); break;*/
			
			//Hotel
		case kSingleRoomType:		instance = new SingleRoomItem(tower); break;
		case kDoubleRoomType:		instance = new DoubleRoomItem(tower); break;
		case kSuiteType:			instance = new SuiteItem(tower); break;
			
			//Services
		/*case kHousekeepingType:		instance = new HousekeepingItem(tower); break;*/
	}
	
	//Initialize the item
	if (instance) {
		instance->setRect(rect);
	}
	
	return instance;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Descriptors
//----------------------------------------------------------------------------------------------------

ItemType Item::getType()
{
	return descriptor->type;
}

ItemGroup Item::getGroup()
{
	return descriptor->group;
}

ItemCategory Item::getCategory()
{
	return descriptor->category;
}

bool Item::isWidthFlexible()
{
	return (descriptor->attributes & kFlexibleWidthAttribute);
}



ItemDescriptor * Item::descriptorForItemType(ItemType itemType)
{
	switch (itemType) {
			//Structure
		case kLobbyType:			return &LobbyItem::descriptor; break;
		case kFloorType:			return &floorItemDescriptor; break;
		case kStairsType:			return &StairsItem::descriptor; break;
		case kEscalatorType:		return &EscalatorItem::descriptor; break;
			
			//Elevator
		/*case kStandardElevatorType:	return &StandardElevatorItem::descriptor; break;
			
			//Office
		case kOfficeType:			return &OfficeItem::descriptor; break;*/
			
			//Hotel
		case kSingleRoomType:		return &SingleRoomItem::descriptor; break;
		case kDoubleRoomType:		return &DoubleRoomItem::descriptor; break;
		case kSuiteType:			return &SuiteItem::descriptor; break;
			
			//Services
		/*case kHousekeepingType:		return &HousekeepingItem::descriptor; break;*/
	}
	return NULL;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Location
//----------------------------------------------------------------------------------------------------

const recti & Item::getRect() const
{
	return rect;
}

recti Item::getFloorRect(int floor) const
{
	recti r = getRect();
	r.size.y = 1;
	r.origin.y = floor;
	return r;
}

void Item::setRect(const recti & rect)
{
	if (this->rect != rect) {
		willChangeRect(rect);
		this->rect = rect;
		didChangeRect();
		setWorldRect(tower->structure->cellToWorld(rect));
	}
}

const rectd & Item::getWorldRect() const
{
	return worldRect;
}

rectd Item::getFloorWorldRect(int floor)
{
	rectd r = getWorldRect();
	r.size.y /= getNumFloors();
	r.origin.y += r.size.y * (floor - getMinFloor());
	return r;
}

void Item::setWorldRect(const rectd & worldRect)
{
	if (this->worldRect != worldRect) {
		willChangeWorldRect(worldRect);
		this->worldRect = worldRect;
		didChangeWorldRect();
	}
}

void Item::didChangeWorldRect()
{
	//Since the world rect changed we have to update the background and the construction.
	updateBackgroundIfNeeded.setNeeded();
	updateConstructionIfNeeded.setNeeded();
}

rectmaski Item::getOccupiedRectMask()
{
	if (descriptor && !descriptor->mask.empty())
		return descriptor->mask.offsetRectMask(getRect().origin);
	return rectmaski(&getRect(), NULL);
}

unsigned int Item::getNumFloors() const
{
	return getRect().size.y;
}

int Item::getMaxFloor() const
{
	return getRect().maxY() - 1;
}

int Item::getMinFloor() const
{
	return getRect().minY();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

bool Item::isConstructed()
{
	return constructed;
}

void Item::setConstructed(bool c)
{
	if (constructed != c) {
		constructed = c;
		updateConstructionIfNeeded.setNeeded();
	}
}



void Item::advance(double dt)
{
	//If we're not yet constructed, advance the construction.
	if (!isConstructed() && !tower->structure->areConstructionsHalted())
		advanceConstruction(dt);
	
	//If we are constructed, advance the item
	if (isConstructed())
		advanceItem(dt);
	
	GameObject::advance(dt);
}

void Item::advanceConstruction(double dt)
{
	//Advance the construction progress.
	constructionProgress += dt * 1.0;
	
	//If the construction is done or this is a flexible item, set constructed to true.
	if (constructionProgress >= 1.0 || isWidthFlexible()) {
		constructionProgress = 1.0;
		setConstructed(true);
	}
	
	//If we're not yet constructed, update the construction which shuffles the worker sprites.
	if (!isConstructed() && shouldAdvance("workers", 0.1))
		updateConstructionIfNeeded.setNeeded();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

void Item::update()
{
	//Update the item if needed
	updateItemIfNeeded();
	
	//Update the construction
	updateConstructionIfNeeded();
	
	GameObject::update();
}

void Item::updateConstruction()
{
	//If we aren't under construction we have to get rid of the construction workers.
	if (isConstructed()) {
		constructionWorkers.clear();
		constructionTexture = NULL;
	}
	
	//Otherwise we have to make sure that we have one construction worker for each floor.
	else {
		//Do the same as with backgrounds. Determine the number of workers first.
		unsigned int numberOfWorkers = (isWidthFlexible() ? 0 : getNumFloors());
		
		//Erase the superfluous workers.
		while (constructionWorkers.size() > numberOfWorkers && !constructionWorkers.empty())
			constructionWorkers.erase(constructionWorkers.begin());
		
		//Add missing workers.
		for (unsigned int i = 0; i < numberOfWorkers; i++) {
			if (!constructionWorkers[i])
				constructionWorkers[i] = new Sprite;
		}
		
		//Now we have to position each individual worker and make sure it shows an animation sprite.
		for (FloorSpriteMap::iterator it = constructionWorkers.begin();
			 it != constructionWorkers.end(); it++) {
			
			//Calculate the worker's new rect
			recti rect;
			rect.size = int2(2, 1);
			rect.origin.x = getRect().minX() + randi(0, getRect().size.x - rect.size.x);
			rect.origin.y = getMinFloor() + it->first;
			
			//Convert to world coordinates and adjust the height
			rectd worldRect = tower->structure->cellToWorld(rect);
			worldRect.size.y = 24;
			
			//Set the worker's rect
			it->second->rect = worldRect;
			
			//Load the worker's texture
			it->second->texture = Texture::named("simtower/construction/workers");
			
			//Calculate the new texture rect
			rectd texrect;
			texrect.size = double2(1.0 / 6, 1);
			texrect.origin.x = randi(0, 5) * texrect.size.x;
			
			//Set the new texture rect
			it->second->textureRect = texrect;
		}
		
		//Load the appropriate construction texture
		stringstream textureName;
		textureName << "simtower/construction/";
		textureName << (isWidthFlexible() ? "floor" : "facility");
		constructionTexture = Texture::named(textureName.str());
	}
}

void Item::updateItem()
{
	//Update the background if required
	updateBackgroundIfNeeded();
}

void Item::updateBackground()
{
	//Decide how many background sprites we need to have.
	unsigned int numberOfBackgrounds = (hasUnifiedBackground() ? 1 : getNumFloors());
	
	//Make sure we have the right number of background sprites, which is one for each floor. We do
	//this by removing superfluous sprites first.
	while (backgrounds.size() > numberOfBackgrounds && !backgrounds.empty())
		backgrounds.erase(backgrounds.begin());
	
	//Then we add fresh new sprites for missing floors.
	for (unsigned int i = 0; i < numberOfBackgrounds; i++) {
		if (!backgrounds[i])
			backgrounds[i] = new Sprite;
	}
	
	
	//Now we may actually start laying out the background sprites. If we're supposed to have a uni-
	//fied background it's very simple. The background simply covers the entire item.
	if (hasUnifiedBackground()) {
		backgrounds[0]->rect = getWorldRect();
	}
	
	//Otherwise we have to give each background sprite its appropriate rect.
	else {
		//Iterate through the background rects. and assign each the appropriate floor.
		for (FloorSpriteMap::iterator it = backgrounds.begin(); it != backgrounds.end(); it++)
			backgrounds[it->first]->rect = getFloorWorldRect(it->first + getMinFloor());
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

bool Item::hasUnifiedBackground()
{
	return unifiedBackground;
}

void Item::setUnifiedBackground(bool ub)
{
	if (unifiedBackground != ub) {
		unifiedBackground = ub;
		updateBackgroundIfNeeded.setNeeded();
	}
}



void Item::draw(rectd dirtyRect)
{
	//If we're not yet constructed draw the construction.
	if (!isConstructed())
		drawConstruction(dirtyRect);
	
	//Otherwise draw the item
	else
		drawItem(dirtyRect);
	
	GameObject::draw(dirtyRect);
}

void Item::drawConstruction(rectd dirtyRect)
{
	//Create a textured quad which we will use to render the construction texture for each floor.
	TexturedQuad quad;
	quad.texture = constructionTexture;
	
	//Iterate through the background sprites and draw the textured quad at each one's location.
	for (FloorSpriteMap::iterator it = backgrounds.begin(); it != backgrounds.end(); it++) {
		
		//Set the quad's rect to be the background's one
		quad.rect = it->second->rect;
		
		//Calculate the offset relative to which the texture is going to be drawn. Since we want to
		//have the texture relative to the item if it's not flexible, we have to set the offset to
		//the quad's origin.
		double2 offset;
		if (!isWidthFlexible())
			offset = quad.rect.origin;
		
		//Autogenerate the texture rectangle.
		quad.autogenerateTextureRect(true, false, offset);
		
		//Draw the quad
		quad.draw();
	}
	
	//Iterate through the construction workers and draw each
	for (FloorSpriteMap::iterator it = constructionWorkers.begin();
		 it != constructionWorkers.end(); it++)
		it->second->draw();
}

void Item::drawItem(rectd dirtyRect)
{
	//Draw the background
	drawBackground(dirtyRect);
	
	//Draw the people
	drawPeople(dirtyRect);
}

void Item::drawBackground(rectd dirtyRect)
{
	//Iterate through the backgrounds and draw each
	for (FloorSpriteMap::iterator it = backgrounds.begin(); it != backgrounds.end(); it++)
		it->second->draw();
}

void Item::drawPeople(rectd dirtyRect)
{
	//Iterate through the people that are currently at this item and draw the ones that want to be.
	for (People::iterator it = people.begin(); it != people.end(); it++)
		(*it)->drawAnimation(dirtyRect);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark People
//----------------------------------------------------------------------------------------------------

void Item::addPerson(Person * person)
{
	willAddPerson(person);
	people.insert(person);
	didAddPerson(person);
}

void Item::removePerson(Person * person)
{
	willRemovePerson(person);
	people.erase(person);
	didRemovePerson(person);
}
