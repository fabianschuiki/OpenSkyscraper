#ifndef OSS_CLASSIC_TOWER_TOWER_H
#define OSS_CLASSIC_TOWER_TOWER_H

#include "../game.h"


namespace OSS {
	namespace Classic {
		class Item;
		
		class TowerBackground;
		class TowerDecoration;
		class TowerEnvironment;
		class TowerFunds;
		class TowerStructure;
		class TowerTime;
		
		class Tower : public SceneObject, public Responder {
			
			/**
			 * Construction
			 */
		public:
			Tower();
			
			
			/**
			 * Dimensions
			 */
			
			int2 cellSize;
			int ceilingHeight;
			recti bounds;
			
			//Rects
			recti convertWorldToCellRect(rectd rect);
			rectd convertCellToWorldRect(recti rect);
			
			//Coordinates
			int2 convertWorldToCellCoordinates(double2 coordinates);
			double2 convertCellToWorldCoordinates(int2 coordinates);
			
			//Ground Floor
			recti getGroundFloorRect() const;
			
			
			/**
			 * Notifications
			 */
			void onChangeTransportItems();
			
			
			/**
			 * Subsystems
			 */
		public:
			Pointer<TowerTime> time;
			Pointer<TowerDecoration> decoration;
			Pointer<TowerEnvironment> environment;
			Pointer<TowerFunds> funds;
			Pointer<TowerBackground> background;
			Pointer<TowerStructure> structure;
			
			
			/**
			 * Simulation
			 */
		public:
			void advance(double dt);
			
			
			/**
			 * State
			 */
		public:
			virtual void update();
			
			
			/**
			 * Drawing
			 */
		public:
			virtual void draw(rectd dirtyRect);
			
			
			/**
			 * Events
			 */
		public:
			virtual bool sendEventToNextResponders(OSS::Event * event);
		};
	}
}


//Include the forward-declared stuff
#include "../items/item.h"

#include "background.h"
#include "decoration.h"
#include "environment.h"
#include "funds.h"
#include "structure.h"
#include "time.h"


#endif
