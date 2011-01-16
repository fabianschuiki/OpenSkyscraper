#ifndef OSS_CLASSIC_ITEMS_ITEM_H
#define OSS_CLASSIC_ITEMS_ITEM_H

#include "itemdescriptor.h"
#include "../tower/tower.h"


namespace OSS {
	namespace Classic {		
		class Person;
		
		class Item : public GameObject {			
			
			/**
			 * Initialization
			 */
		public:
			const Pointer<Tower> tower;
			
			Item(Tower * tower, ItemDescriptor * descriptor);
			
			bool isInTower();
			
			//Factory
			static Item * make(Tower * tower, ItemDescriptor * descriptor, recti rect);
			
			
			/**
			 * Descriptor
			 */
		public:
			const ItemDescriptor * descriptor;
			
			ItemType getType();
			ItemGroup getGroup();
			ItemCategory getCategory();
			bool isWidthFlexible();
			
			static ItemDescriptor * descriptorForItemType(ItemType itemType);
			
			
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
			
			virtual void willChangeRect(recti newRect) {}
			virtual void didChangeRect() {}
			
			const rectd & getWorldRect() const;
			rectd getFloorWorldRect(int floor);
			void setWorldRect(const rectd & worldRect);
			
			virtual void willChangeWorldRect(rectd newRect) {}
			virtual void didChangeWorldRect();
			
			rectmaski getOccupiedRectMask();
			
			//Convenience
			unsigned int getNumFloors() const;
			int getMaxFloor() const;
			int getMinFloor() const;
			
			
			/**
			 * Simulation
			 */
		private:
			map<string, double> advanceTimestamps;
			double advanceTime;
			
			//Construction
		protected:
			bool constructed;
			double constructionProgress;
			
		public:
			bool shouldAdvance(string identifier, double period);
			
			bool isConstructed();
			void setConstructed(bool c);
			
			virtual void advance(double dt);
			virtual void advanceConstruction(double dt);
			virtual void advanceItem(double dt) {}
			
			
			/**
			 * State
			 */
		public:
			virtual void update();
			virtual void updateConstruction();
			virtual void updateItem();
			virtual void updateBackground();
			
			Updatable::Conditional<Item> updateConstructionIfNeeded;
			Updatable::Conditional<Item> updateItemIfNeeded;
			Updatable::Conditional<Item> updateBackgroundIfNeeded;
			
			
			/**
			 * Drawing
			 */
		protected:
			typedef map<unsigned int, Pointer<Sprite> > FloorSpriteMap;
			FloorSpriteMap backgrounds;
			
		private:
			bool unifiedBackground;
			
			Pointer<Texture> constructionTexture;
			FloorSpriteMap constructionWorkers;
			
		public:
			bool hasUnifiedBackground();
			void setUnifiedBackground(bool ub);
			
			virtual void draw(rectd dirtyRect);
			virtual void drawConstruction(rectd dirtyRect);
			virtual void drawItem(rectd dirtyRect);
			virtual void drawBackground(rectd dirtyRect);
			virtual void drawPeople(rectd dirtyRect);
			
			
			/**
			 * People
			 */
		protected:
			typedef std::set<Person *> People;
			People people;
			
		public:
			void addPerson(Person * person);
			void removePerson(Person * person);
			
			virtual void willAddPerson(Person * person) {}
			virtual void didAddPerson(Person * person) {}
			
			virtual void willRemovePerson(Person * person) {}
			virtual void didRemovePerson(Person * person) {}
		};
	}
}


#include "../people/person.h"


#endif
