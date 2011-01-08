#include "item.h"

using namespace OSS;
using namespace Classic;


static Item::Descriptor floorItemDescriptor = {
	Item::kFloorType,
	Item::kStructureGroup,
	Item::kFacilityCategory,
	1,
	(Item::kFlexibleWidthAttribute),
	500,
	int2(1, 1),
	int2(1, 1)
};





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

Item::Item(Tower * tower, Descriptor * descriptor) : Responder(), tower(tower), descriptor(descriptor)
{
	assert(tower && descriptor);
	
	hasUnionBackground = false;
}

Item::~Item()
{
}

void Item::init()
{
	//Initialize the item ID to 0
	itemID = 0;
	
	//TODO: Move this to its appropriate function
	constructionWorkerUpdateTimer = 0;
	constructionProgress = 0;
	underConstruction = false;
	drawFlexibleConstruction = false;
	
	//Initialize the basic sprites
	initBasicSprites();
	
	//Perform the initial update
	update();
}

void Item::update()
{
	//Update the basic sprites
	updateBasicSprites();
}
															 
															 



//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Factory
//----------------------------------------------------------------------------------------------------

Item * Item::make(Tower * tower, Descriptor * descriptor)
{
	if (!tower || !descriptor)
		return NULL;
	
	//Allocate the new item according to the descriptor type
	Item * instance = NULL;
	switch (descriptor->type) {
			//Structure
		case Item::kLobbyType:		instance = new LobbyItem(tower); break;
		case Item::kStairsType:		instance = new StairsItem(tower); break;
		case Item::kEscalatorType:	instance = new EscalatorItem(tower); break;
			
			//Elevator
		case Item::kStandardElevatorType:	instance = new StandardElevatorItem(tower); break;
			
			//Office
		case Item::kOfficeType:		instance = new OfficeItem(tower); break;
			
			//Hotel
		case Item::kSingleRoomType:	instance = new SingleRoomItem(tower); break;
		case Item::kDoubleRoomType:	instance = new DoubleRoomItem(tower); break;
		case Item::kSuiteType:		instance = new SuiteItem(tower); break;
			
			//Services
		case Item::kHousekeepingType:	instance = new HousekeepingItem(tower); break;
	}
	
	//Initialize the item
	if (instance)
		instance->init();
	
	return instance;
}

Item * Item::make(Tower * tower, Descriptor * descriptor, unsigned int itemID)
{
	Item * instance = make(tower, descriptor);
	if (instance)
		instance->setItemID(itemID);
	return instance;
}

Item * Item::make(Tower * tower, Descriptor * descriptor, unsigned int itemID, recti rect)
{
	Item * instance = make(tower, descriptor, itemID);
	if (instance)
		instance->setRect(rect);
	return instance;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Basic Attributes
//----------------------------------------------------------------------------------------------------

Item::Type Item::getType()
{
	return descriptor->type;
}

Item::Group Item::getGroup()
{
	return descriptor->group;
}

Item::Category Item::getCategory()
{
	return descriptor->category;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Identification
//----------------------------------------------------------------------------------------------------

unsigned int Item::getItemID() const
{
	return itemID;
}

void Item::setItemID(unsigned int itemID)
{
	this->itemID = itemID;
}

bool Item::isValid() const
{
	return (getItemID() != 0);
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
		this->rect = rect;
		setWorldRect(tower->convertCellToWorldRect(rect));
	}
}

const rectd & Item::getWorldRect() const
{
	return worldRect;
}

void Item::setWorldRect(const rectd & worldRect)
{
	if (this->worldRect != worldRect) {
		this->worldRect = worldRect;
		onChangeLocation();
	}
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
#pragma mark Basic Sprites
//----------------------------------------------------------------------------------------------------

void Item::initBasicSprites()
{
	initBackground();
}

void Item::updateBasicSprites()
{
	updateBackground();
}

void Item::initBackground()
{
}

void Item::updateBackground()
{	
	//If we're supposed to only have one big background, set its rect accordingly
	if (hasUnionBackground) {
		backgrounds[0].setRect(getWorldRect());
	}
	
	//Otherwise calculate the background sprite rects
	else {
		for (int floor = 0; floor < getNumFloors(); floor++) {
			//Calculate the rect in cell coordinates
			recti rect = getRect();
			rect.size.y = 1;
			rect.origin.y += floor;
			
			//Convert the rect to world coordinates
			rectd worldRect = tower->convertCellToWorldRect(rect);
			
			//Set the background rect
			backgrounds[floor].setRect(worldRect);
		}
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Descriptors
//----------------------------------------------------------------------------------------------------

Item::Descriptor * Item::descriptorForItemType(Item::Type itemType)
{
	switch (itemType) {
			//Structure
		case Item::kLobbyType:		return &LobbyItem::descriptor; break;
		case Item::kFloorType:		return &floorItemDescriptor; break;
		case Item::kStairsType:		return &StairsItem::descriptor; break;
		case Item::kEscalatorType:	return &EscalatorItem::descriptor; break;
			
			//Elevator
		case Item::kStandardElevatorType:	return &StandardElevatorItem::descriptor; break;
			
			//Office
		case Item::kOfficeType:		return &OfficeItem::descriptor; break;
			
			//Hotel
		case Item::kSingleRoomType:	return &SingleRoomItem::descriptor; break;
		case Item::kDoubleRoomType:	return &DoubleRoomItem::descriptor; break;
		case Item::kSuiteType:		return &SuiteItem::descriptor; break;
			
			//Services
		case Item::kHousekeepingType:	return &HousekeepingItem::descriptor; break;
	}
	return NULL;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void Item::advance(double dt)
{
	//Simulate the construction
	if (constructionProgress < 1.0) {
		//Increase the construction worker update timer
		constructionWorkerUpdateTimer += dt;
		if (constructionWorkerUpdateTimer > 0.1) {
			constructionWorkerUpdateTimer -= 0.1;
			updateConstructionWorkerSprites();
		}
		
		//Advance the construction progress
		if (!tower->getConstructionsHalted()) {
			constructionProgress += dt * 1.0;
			if (constructionProgress >= 1.0 || drawFlexibleConstruction)
				setUnderConstruction(false);
		}
	}
	
	//If the date advanced, issue a notification
	if (tower->didDateAdvance())
		onDateAdvance();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void Item::draw(rectd visibleRect)
{
	//Draw the construction
	if (constructionSprite) {
		constructionSprite->rect = (drawFlexibleConstruction ? worldRect : backgrounds[0].rect);
		constructionSprite->autoTexRelativeX = !drawFlexibleConstruction;
		constructionSprite->draw(visibleRect);
	}
	
	//Draw the construction workers
	if (!drawFlexibleConstruction)
		for (int i = 0; i < 3; i++)
			if (constructionWorkerSprite[i])
				constructionWorkerSprite[i]->draw(visibleRect);
	
	//Draw the background sprites
	if (!underConstruction) {
		if (hasUnionBackground)
			backgrounds[0].draw(visibleRect);
		else
			for (int i = 0; i < getRect().size.y; i++)
				backgrounds[i].draw(visibleRect);
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Notifications
//----------------------------------------------------------------------------------------------------

void Item::onChangeLocation()
{
	updateBasicSprites();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Uncategorized
//----------------------------------------------------------------------------------------------------

void Item::setUnderConstruction(bool uc)
{
	//Ignore if the construction state didn't change
	if (underConstruction == uc) return;
	underConstruction = uc;
	
	//Switch to construction mode
	if (underConstruction) {
		//Setup the construction sprite if required
		if (!constructionSprite) {
			constructionSprite = new Sprite;
			constructionSprite->textureMode = Sprite::kRepeatTextureMode;
			constructionSprite->autoTexRectX = true;
			
			//Depending on whether the item is draggable the construction sprite differs
			drawFlexibleConstruction = (this->descriptor->attributes & kFlexibleWidthAttribute);
			std::string textureName = "simtower/construction/";
			textureName += (drawFlexibleConstruction ? "floor" : "facility");
			constructionSprite->texture = Texture::named(textureName);
		}
		
		//Setup the construction
		constructionProgress = 0;
		
		//Setup the construction worker sprites
		for (int i = 0; i < 3; i++) {
			if (!constructionWorkerSprite[i] && rect.size.y > i) {
				Sprite * sprite = new Sprite;
				sprite->rect.size = double2(16, 24);
				sprite->texture = Texture::named("simtower/construction/workers");
				sprite->textureRect.size = double2(1.0 / 6, 1);
				constructionWorkerSprite[i] = sprite;
			}
		}
		updateConstructionWorkerSprites();
	}
	
	//Switch to constructed mode
	else {
		constructionSprite = NULL;
		constructionProgress = 1.0;
		
		//Destroy construction worker sprites
		for (int i = 0; i < 3; i++)
			constructionWorkerSprite[i] = NULL;
	}
}

void Item::updateConstructionWorkerSprites()
{
	for (int i = 0; i < 3; i++) {
		Sprite * sprite = constructionWorkerSprite[i];
		if (!sprite) continue;
		
		//Position the sprite
		int2 newCoords(rand() % (rect.size.x - 1), i);
		sprite->rect.origin = tower->convertCellToWorldCoordinates(newCoords + rect.origin);
		
		//Choose a new texture area
		sprite->textureRect.origin.x = (rand() % 6) * sprite->textureRect.size.x;
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark People
//----------------------------------------------------------------------------------------------------

void Item::addPerson(Person * person)
{
	people.insert(person);
}

void Item::removePerson(Person * person)
{
	people.erase(person);
}
