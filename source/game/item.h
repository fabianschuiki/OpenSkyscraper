#ifndef OSS_ITEM_H
#define OSS_ITEM_H

#include "../general.h"
#include "../base/base.h"
#include "../core/coreobject.h"
#include "../core/sprite.h"


namespace OSS {
	class Tower;
	class Person;
	
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
			rectmaski mask;
		} Descriptor;
		
		
		/**
		 * Initialization
		 */
	public:
		Item(Tower * tower, Descriptor * descriptor);
		virtual ~Item();
		
		//Note that the init() function is separate from the constructor since we want to be able
		//to initialize the entire class hierarchy of the item at once. Calling virtual methods in
		//the constructor fails.
		virtual void init();	//calls the initialization tree
		virtual void update();	//calls the update tree
		
		//Factory
		static Item * make(Tower * tower, Descriptor * descriptor);
		static Item * make(Tower * tower, Descriptor * descriptor,
						   unsigned int itemID);
		static Item * make(Tower * tower, Descriptor * descriptor,
						   unsigned int itemID, recti rect);
		
		
		/**
		 * Basic Attributes
		 */
		const Pointer<Tower> tower;
		const Descriptor * descriptor;
		Type getType();
		Group getGroup();
		Category getCategory();
		
		
		/**
		 * Identification
		 */
	private:
		unsigned int itemID;
	public:
		unsigned int getItemID() const;
		void setItemID(unsigned int itemID);
		bool isValid() const;
		
		
		/**
		 * Location
		 */
	private:
		recti rect;
		rectd worldRect;
	public:
		const recti & getRect() const;
		recti getFloorRect(int floor) const;
		void setRect(const recti & rect);
		const rectd & getWorldRect() const;
		void setWorldRect(const rectd & worldRect);
		
		//Convenience
		unsigned int getNumFloors() const;
		int getMaxFloor() const;
		int getMinFloor() const;
		
		
		/**
		 * Basic Sprites
		 */
		virtual void initBasicSprites();
		virtual void updateBasicSprites();
		
		std::map<unsigned int, Sprite> backgrounds;
		virtual void initBackground();
		virtual void updateBackground();
		
		
		//Construction Process
	protected:
		Pointer<Sprite> constructionSprite;
		Pointer<Sprite> constructionWorkerSprite[3];
		double constructionWorkerUpdateTimer;
		double constructionProgress;
		bool underConstruction;
		bool drawFlexibleConstruction;
	public:
		void setUnderConstruction(bool uc);
		void updateConstructionWorkerSprites();
		
		//Descriptors
		static Descriptor * descriptorForItemType(Type itemType);
		
		//Simulation
		virtual void advance(double dt);
		
		//Drawing
		virtual void draw(rectd visibleRect);
		
		/**
		 * Notifications
		 */
		
		//Location
		virtual void onChangeLocation();
		
		//Surroundings
		virtual void onChangeTransportItems() {}
		
		
		/**
		 * People
		 */
	protected:
		typedef std::set< Pointer<Person> > People;
		People people;
	public:
		virtual void addPerson(Person * person);
		virtual void removePerson(Person * person);
	};
}


#endif
