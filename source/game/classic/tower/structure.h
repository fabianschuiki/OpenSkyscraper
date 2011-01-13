#ifndef OSS_GAME_CLASSIC_TOWER_STRUCTURE_H
#define OSS_GAME_CLASSIC_TOWER_STRUCTURE_H

#include "tower.h"


namespace OSS {
	namespace Classic {
		class Item;
		
		class TowerStructure : public SceneObject, public Responder {
			
			/**
			 * Construction
			 */
		public:
			const Pointer<Tower> tower;
			
			TowerStructure(Tower * tower);
			
			
			/**
			 * Dimensions
			 */
		public:
			const int2 cellSize;
			const int ceilingHeight;
			
			double2 cellToWorld(int2 v);
			rectd cellToWorld(recti v);
			
			int2 worldToCell(double2 v);
			recti worldToCell(rectd v);
			
			
			/**
			 * Cells
			 *
			 * The tower's structure is being kept track of in a cell map. Each cell represents a
			 * grid cell of the tower and contains a pointer to the facility and the transport that
			 * occupy that cell.
			 */
		public:
			typedef map<ItemCategory, Item *> ItemByCategory;
			typedef struct {
				ItemByCategory items;
				int2 location;
			} Cell;
			
			typedef std::set<Cell *> CellSet;
			
		private:
			map<int, map<int, Cell> > cells;
			
		public:
			Cell * getCell(int2 location, bool createIfInexistent = false);
			
			CellSet getCells(recti rect, bool createIfInexistent = false);
			CellSet getCells(rectmaski rectmask, bool createIfInexistent = false);
			
			//Analysis
			typedef struct {
				unsigned int floor;
				unsigned int facility;
				unsigned int transport;
			} CellAnalysis;
			
			CellAnalysis analyseCells(CellSet cells);
			CellAnalysis analyseCells(recti rect);
			CellAnalysis analyseCells(rectmaski rectmask);
			
			void assignCellsCoveredByItem(Item * item);
			void unassignCellsCoveredByItem(Item * item);
			
			
			/**
			 * Items
			 */
		public:
			typedef std::set< Pointer<Item> > ItemPointerSet;
			typedef std::set<Item *> ItemSet;
			typedef map<ItemType, ItemSet> ItemTypeMap;
			typedef map<ItemGroup, ItemSet> ItemGroupMap;
			typedef map<ItemCategory, ItemSet> ItemCategoryMap;
			
		private:
			ItemPointerSet items;
			ItemTypeMap itemsByType;
			ItemGroupMap itemsByGroup;
			ItemCategoryMap itemsByCategory;
			
			map<int, ItemSet> itemsByFloor;
			map<int, ItemTypeMap> itemsByFloorAndType;
			map<int, ItemGroupMap> itemsByFloorAndGroup;
			map<int, ItemCategoryMap> itemsByFloorAndCategory;
			
		public:
			const ItemPointerSet & getItems();
			const ItemSet & getItems(ItemType type);
			const ItemSet & getItems(ItemGroup group);
			const ItemSet & getItems(ItemCategory category);
			
			const ItemSet & getItems(int floor);
			const ItemSet & getItems(int floor, ItemType type);
			const ItemSet & getItems(int floor, ItemGroup group);
			const ItemSet & getItems(int floor, ItemCategory category);
			
			ItemSet getItems(CellSet cells);
			ItemSet getItems(recti rect);
			ItemSet getItems(rectmaski rectmask);
			
			void addItem(Item * item);
			void removeItem(Item * item);
			
			
			/**
			 * Reports
			 */
		public:
			typedef struct {
				unsigned int additionalFloorCellsRequired;
				unsigned int additionalFacilityCellsRequired;
				
				ItemSet collidesWith;
				unsigned int unfulfilledAttributes;
				
				bool cellsAboveValid;
				bool cellsBelowValid;
				bool adjacentCellsValid;
				
				bool validForFacility;
				bool validForTransport;
				bool valid;
			} Report;
			
			Report getReport(recti rect, ItemDescriptor * descriptor);
			
			
			/**
			 * Item Construction
			 *
			 * The constructItem() function tries to build the item specified by the given item
			 * descriptor in the given rect.
			 */
		private:
			Pointer<SoundEffect> flexibleConstructionSound;
			Pointer<SoundEffect> constructionSound;
			
		public:
			typedef struct {
				bool success;
				string failureReason;
			} ConstructionResult;
			
			ConstructionResult constructItem(ItemDescriptor * descriptor,
											 recti rect, recti initialRect);
			
			
			/**
			 * Simulation
			 */
		public:
			virtual void advance(double dt);
			
			
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
		};
	}
}


#endif
