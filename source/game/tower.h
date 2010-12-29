#ifndef OSS_TOWER_H
#define OSS_TOWER_H

#include "../general.h"
#include "../base/base.h"
#include "../core/core.h"
#include "item.h"


namespace OSS {
	class Tower : public CoreObject {
	public:
		//Initialization
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
		
		
		/**
		 * Notifications
		 */
		void onMoveOnScreen();
		void onMoveOffScreen();
		
		
		/**
		 * Simulation
		 */
		void advance(double dt);
		
		
		/**
		 * Rendering
		 */
		Pointer<Sprite> groundSprite;
		Pointer<Sprite> skySprites[10];
		Pointer<Sprite> citySprite;
		Pointer<Sprite> craneSprite;
		void prepareBackground();
		void renderBackground(rectd visibleRect);
		
		
		/**
		 * Items
		 */
		/*Item * getFacility(unsigned int itemID, bool createIfInexistent = false);
		Item * getTransport(unsigned int itemID, bool createIfInexistent = false);
		
	protected:*/
		typedef std::map< unsigned int, Pointer<Item> > ItemMap;
		ItemMap facilityItems;
		ItemMap transportItems;
		
		unsigned int nextItemID();
		
	private:
		bool constructionsHalted;
	public:
		void setConstructionsHalted(bool halted);
		bool getConstructionsHalted();
		
		
		
		/**
		 * Cells
		 */
		typedef struct {
			unsigned int facility;
			unsigned int transport;
		} Cell;
		
		/**
		 * Returns a pointer to the cell at the given coordinates. If creatIfInexistent is true,
		 * creates the cell in the cells map if it doesn't exist. Otherwise NULL is returned for
		 * cells that don't exist.
		 */
		Cell * getCell(int2 coordinates, bool createIfInexistent = false);
		
	public:
		std::map<int, std::map<int, Cell> > cells;
		
		
		/**
		 * Construction
		 */
		bool constructFlexibleWidthItem(Item::Descriptor * descriptor, recti currentRect, recti previousRect);
		bool constructItem(Item::Descriptor * descriptor, recti rect);
		bool checkIfRectMeetsDescriptorRequirements(Item::Descriptor * descriptor, recti rect);
		void analyzeCellsInRect(recti rect,
								int * numEmptyCells, int * numFloorCells, int * numOccupiedCells,
								int * numOccupiedCellsBelow, int * numOccupiedCellsAbove);
		void insertNewItem(Item::Descriptor * descriptor, recti rect);
		
		
		/**
		 * Uncategorized
		 */
		ALuint buildSoundSource;
	};
}


#endif
