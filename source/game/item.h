#ifndef OSS_ITEM_H
#define OSS_ITEM_H

#include "../general.h"
#include "../base/base.h"
#include "../core/coreobject.h"
#include "../core/sprite.h"


namespace OSS {
	class Tower;
	
	class Item : public CoreObject {
	public:
		//Item Types
		typedef enum {
			kNoneType = 0,
			
			//Structure
			kLobbyType,
			kFloorType,
			kStairsType,
			kEscalatorType,
			
			//Elevator
			kElevatorType,
			kServiceElevatorType,
			kExpressElevatorType,
			
			//Office
			kOfficeType,
			
			//Hotel
			kSingleRoomType,
			kDoubleRoomType,
			kSuiteType,
			
			//Condo
			kCondoType,
			
			//Entertainment
			kFastFoodType,
			kRestaurantType,
			kShopType,
			kCinemaType,
			kPartyHall,
			
			//Infrastructure
			kParkingRampType,
			kParkingSpaceType,
			kRecyclingCenterType,
			kMetroType,
			
			//Services
			kCathedrakType,
			kSecurityType,
			kMedicalCenterType,
			kHousekeepingType,
			
			kMaxType
		} Type;
		
		//Item Groups
		typedef enum {
			kStructureGroup,
			kElevatorGroup,
			kOfficeGroup,
			kHotelGroup,
			kEntertainmentGroup,
			kInfrastructureGroup,
			kServicesGroup
		} Group;
		
		//Item Categories
		typedef enum {
			kFacilityCategory,
			kTransportCategory
		} Category;
		
		//Item Attributes
		enum {
			kFlexibleWidthAttribute		= (1 << 0),
			kEvery15thFloorAttribute	= (1 << 1),
			kNotAboveGroundAttribute	= (1 << 2),
			kNotBelowGroundAttribute	= (1 << 3),
			kAllowedOnGroundAttribute	= (1 << 4)
		};
		
		//Item Descriptor
		typedef struct {
			Type type;
			Group group;
			Category category;
			unsigned short minRating;
			unsigned short attributes;
			unsigned short price;
			int2 cells;
			int2 minUnit;
		} Descriptor;
		
		
		//Tower
	protected:
		Pointer<Tower> tower;
	public:
		void setTower(Tower * tower);
		
		//Attributes
		unsigned int itemID;
		Descriptor * descriptor;
		recti rect;
		rectd worldRect;
		
		//Basic Sprites
		Pointer<Sprite> ceilingSprite;
		Pointer<Sprite> backgroundSprite;
		
		//Construction Process
	private:
		Pointer<Sprite> constructionSprite;
		Pointer<Sprite> constructionWorkerSprite[3];
		double constructionWorkerUpdateTimer;
		double constructionProgress;
		bool underConstruction;
		bool drawFlexibleConstruction;
	public:
		void setUnderConstruction(bool uc);
		void updateConstructionWorkerSprites();
		
		//Initialization
		static Item * createNew(Descriptor * descriptor, recti rect, unsigned int itemID);
		Item();
		~Item();
		
		//Descriptors
		static Descriptor * descriptorForItemType(Type itemType);
		
		//Simulation
		virtual void advance(double dt);
		
		//Rendering
		virtual void draw(rectd visibleRect);
		
		//Notifications
		virtual void onPrepare();
	};
}


#endif
