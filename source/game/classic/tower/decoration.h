#ifndef OSS_CLASSIC_TOWER_DECORATION_H
#define OSS_CLASSIC_TOWER_DECORATION_H

#include "tower.h"


namespace OSS {
	namespace Classic {
		class TowerDecoration : public GameObject {
			
			/**
			 * Initialization
			 */
		public:
			const Pointer<Tower> tower;
			
			TowerDecoration(Tower * tower);
			
			
			/**
			 * State
			 */
		private:
			vector<int> fireLaddersNeedingUpdate;
			
		public:
			virtual void update();
			virtual void updateCrane();
			virtual void updateFireLadders();
			
			Updatable::Conditional<TowerDecoration> updateCraneIfNeeded;
			Updatable::Conditional<TowerDecoration> updateFireLaddersIfNeeded;
			
			
			/**
			 * Drawing
			 */
		private:
			Pointer<Sprite> craneSprite;
			
			typedef std::pair<Pointer<Sprite>, Pointer<Sprite> > SpritePair;
			typedef map<int, SpritePair> SpritePairMap;
			SpritePairMap fireLadders;
			
			Sprite * makeNewFireLadderSprite();
			
		public:
			
			virtual void draw(rectd dirtyRect);
			virtual void drawCrane(rectd dirtyRect);
			virtual void drawFireLadders(rectd dirtyRect);
			
			
			/**
			 * Events
			 */
		public:
			void eventBoundsChanged(Event * event);
			void eventFloorRangeChanged(FloorEvent * event);
		};
	}
}


#endif
