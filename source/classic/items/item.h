#ifndef OSS_CLASSIC_ITEMS_ITEM_H
#define OSS_CLASSIC_ITEMS_ITEM_H

#include "../external.h"
#include "../responder.h"

#include "itemdescriptor.h"
#include "../tower/tower.h"


namespace OSS {
	namespace Classic {		
		class Person;
		
		class Item : public Responder {			
			
			/**
			 * Initialization
			 */
		public:
			Item(Tower * tower, ItemDescriptor * descriptor);
			virtual ~Item();
			
			//Note that the init() function is separate from the constructor since we want to be able
			//to initialize the entire class hierarchy of the item at once. Calling virtual methods in
			//the constructor fails.
			virtual void init();	//calls the initialization tree
			virtual void update();	//calls the update tree
			
			//Factory
			static Item * make(Tower * tower, ItemDescriptor * descriptor);
			static Item * make(Tower * tower, ItemDescriptor * descriptor,
							   unsigned int itemID);
			static Item * make(Tower * tower, ItemDescriptor * descriptor,
							   unsigned int itemID, recti rect);
			
			
			/**
			 * Basic Attributes
			 */
			const Pointer<Tower> tower;
			const ItemDescriptor * descriptor;
			ItemType getType();
			ItemGroup getGroup();
			ItemCategory getCategory();
			
			
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
			rectmaski getOccupiedRectMask();
			
			//Convenience
			unsigned int getNumFloors() const;
			int getMaxFloor() const;
			int getMinFloor() const;
			
			
			/**
			 * Basic Sprites
			 */
			virtual void initBasicSprites();
			virtual void updateBasicSprites();
			
			//Backgrounds
		protected:
			map<unsigned int, Engine::Sprite> backgrounds;
			bool hasUnionBackground;
		public:
			virtual void initBackground();
			virtual void updateBackground();
			
			
			//Construction Process
		protected:
			Pointer<Engine::Sprite> constructionSprite;
			Pointer<Engine::Sprite> constructionWorkerSprite[3];
			double constructionWorkerUpdateTimer;
			double constructionProgress;
			bool underConstruction;
			bool drawFlexibleConstruction;
		public:
			void setUnderConstruction(bool uc);
			void updateConstructionWorkerSprites();
			
			//Descriptors
			static ItemDescriptor * descriptorForItemType(ItemType itemType);
			
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
			
			//Timing
			virtual void onDateAdvance() {}
			
			
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
}


#endif
