#ifndef TOWER_H
#define TOWER_H

#include "../general.h"
#include "../base/base.h"


namespace OSS {
	class Tower : public Object {
	public:
		//Initialization
		Tower();
		
		/**
		 * Dimensions
		 */
		
		int2 cellSize;
		
		//Rects
		recti convertWorldToCellRect(rectd rect);
		rectd convertCellToWorldRect(recti rect);
		
		//Coordinates
		int2 convertWorldToCellCoordinates(double2 coordinates);
		double2 convertCellToWorldCoordinates(int2 coordinates);
		
		
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
	};
}


#endif
