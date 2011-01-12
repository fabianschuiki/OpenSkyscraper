#ifndef OSS_CLASSIC_PEOPLE_PERSON_H
#define OSS_CLASSIC_PEOPLE_PERSON_H

#include "../external.h"

#include "../items/item.h"
#include "../tower/tower.h"
#include "../tower/route.h"


namespace OSS {
	namespace Classic {		
		class Person : public Core::Object {
		protected:
			const Pointer<Tower> tower;
			
		public:
			//Initialization
			Person(Tower * tower);
			virtual void reset();
			
			/**
			 * Attributes
			 */
			
			//Type
		public:
			typedef enum {
				kManType,
				kSalesmanType,
				kChildType,
				kSecurityOfficerType,
				
				kWomanAType,
				kWomanBType,
				kMotherWithChildType,
				kMotherType,
				kJanitorType
			} Type;
			
		private:
			Type type;
			
		public:
			Type getType();
			void setType(Type type);
			virtual void onChangeType();
			typedef enum {
				kMale,
				kFemale
			} Gender;
			Gender getGender();
			
			//Stress
		private:
			double stress;
		public:
			double getStress();
			void setStress(double stress);
			virtual void onChangeStress();
			
			
			/**
			 * Managed Sprites
			 */
		public:
			typedef enum {
				kPortraitType,
				kQueuingType,
				kMountingType,
				kDescendingType
			} SpriteType;
			
			typedef enum {
				kHeadingLeft,
				kHeadingRight
			} SpriteHeading;
			
		private:
			typedef std::set< Pointer<Engine::Sprite> > SpriteSet;
			SpriteSet managedSprites;
			
			typedef map<Engine::Sprite *, SpriteType> SpriteTypeMap;
			SpriteTypeMap managedSpriteTypes;
			
			typedef map<Engine::Sprite *, SpriteHeading> SpriteHeadingMap;
			SpriteHeadingMap managedSpriteHeadings;
			
		public:
			void addManagedSprite(Engine::Sprite * sprite, SpriteType type, SpriteHeading heading);
			void removeManagedSprite(Engine::Sprite * sprite);
			
			void setManagedSpriteType(Engine::Sprite * sprite, SpriteType type);
			void setManagedSpriteHeading(Engine::Sprite * sprite, SpriteHeading heading);
			
			void initManagedSprite(Engine::Sprite * sprite);
			void updateManagedSprite(Engine::Sprite * sprite);
			void updateManagedSprites();
			
			
			/**
			 * Animation Sprite
			 */
		protected:
			Engine::Sprite animationSprite;
			double animationTime;
			unsigned int animationIndex;
			int2 animationLocation;
		public:
			Engine::Sprite & getAnimationSprite();
			
			unsigned int getAnimationIndex();
			void setAnimationIndex(unsigned int animationIndex);
			virtual void onChangeAnimationIndex();
			
			const int2 & getAnimationLocation();
			void setAnimationLocation(int2 animationLocation);
			virtual void onChangeAnimationLocation();
			
			virtual void initAnimationSprite();
			virtual void updateAnimationSprite();
			
			virtual double getAnimationPeriod();
			virtual bool shouldAnimate();
			void resetAnimation();
			void advanceAnimation(double dt);
			virtual void shuffleAnimation();
			void drawAnimation(rectd visibleRect);
			
			
			/**
			 * Location
			 */
		private:
			int floor;
			Pointer<Item> item;
			double arrivalTime;
		public:
			//Floor
			int getFloor();
			void setFloor(int floor);
			
			//Item
			Item * getItem() const;
			void setItem(Item * item);
			
			//Arrival time
			double getArrivalTime();
			void setArrivalTime(double time);
			
			//Convenience
			double getTimeSinceArrival();
			bool isAt(Item * item);
			bool hasBeenAtFor(Item * item, double duration);
			
			
			/**
			 * Journey
			 */
		private:
			int nextFloor;
			unsigned int nodeIndex;
		public:
			int getNextFloor();
			void setNextFloor(int nextFloor);
			void initJourney();
			void advanceJourney();
			
			
			/**
			 * Route
			 */
		private:
			Pointer<Item> destination;
			Pointer<Route> route;
		public:
			Item * getDestination();
			void setDestination(Item * destination);
			Route * getRoute();
			void setRoute(Route * route);
			void updateRoute();
			
			//Convenience
			bool isAtDestination();
			bool hasRoute();
			
			
			/**
			 * Notifications
			 */
			virtual void onArrivedAtDestination();
			
			
			/**
			 * Intelligence
			 */
			virtual void update();
			
			
			/**
			 * Uncategorized
			 */
			
			//DEBUG: Properties. These are only used for debuggin purposes and ought to be replaced in
			//later versions of the game by a more sophisticated system.
			map<string, bool> boolProps;
			map<string, int> intProps;
		};
	}
}


#endif
