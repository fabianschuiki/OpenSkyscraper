#ifndef OSS_CLASSIC_TOWER_STRUCTURE_H
#define OSS_CLASSIC_TOWER_STRUCTURE_H

#include "../external.h"


namespace OSS {
	namespace Classic {
		class Item;
		class Tower;
		
		class TowerStructure : public Engine::Object {
			
			/**
			 * Construction
			 */
		public:
			const Pointer<Tower> tower;
			
			TowerStructure(Tower * tower);
			
			
			/**
			 * Cells
			 *
			 * The tower's structure is being kept track of in a cell map. Each cell represents a
			 * grid cell of the tower and contains a pointer to the facility and the transport that
			 * occupy that cell.
			 */
		public:
			typedef struct {
				FacilityItem * facility;
				TransportItem * transport;
			} Cell;
			
		private:
			std::map<int, std::map<int, Cell> > cells;
			
		public:
			Cell * getCell(int2 location, bool createIfInexistent = false);
			
			//Analysis
			typedef struct {
				unsigned int empty;
				unsigned int floor;
				unsigned int facility;
				unsigned int transport;
			} CellAnalysis;
			
			CellAnalysis analyseCells(recti rect);
			CellAnalysis analyseCells(rectmaski rectmask);
			
			
			/**
			 * Items
			 */
		public:
			typedef std::set< Pointer<Item> > ItemSet;
			typedef std::map<ItemType, ItemSet> ItemTypeSet;
			typedef std::map<ItemGroup, ItemSet> ItemGroupSet;
			typedef std::map<ItemCategory, ItemSet> ItemCategorySet;
			
		private:
			ItemSet items;
			ItemTypeSet itemsByType;
			ItemGroupSet itemsByGroup;
			ItemCategorySet itemsByCategory;
			
			std::map<int, ItemSet> itemsByFloor;
			std::map<int, ItemTypeSet> itemsByFloorAndType;
			std::map<int, ItemGroupSet> itemsByFloorAndGroup;
			std::map<int, ItemCategorySet> itemsByFloorAndCategory;
			
		public:
			const ItemSet & getItems();
			const ItemTypeSet & getItems(ItemType type);
			const ItemGroupSet & getItems(ItemGroup group);
			const ItemCategorySet & getItems(ItemCategory category);
			
			const ItemSet & getItems(int floor);
			const ItemTypeSet & getItems(int floor, ItemType type);
			const ItemGroupSet & getItems(int floor, ItemGroup group);
			const ItemCategorySet & getItems(int floor, ItemCategory category);
			
			ItemSet getItems(recti rect);
			ItemSet getItems(rectmaski rectmask);
			
			void addItem(Item * item);
			void removeItem(Item * item);
			
			
			/**
			 * Reports
			 */
		public:
			typedef struct {
				unsigned int floorCellsRequired;
				unsigned int itemCellsRequired;
				
				ItemSet collidesWith;
				unsigned int unfulfilledAttributes;
				bool cellsBelowValid;
				bool cellsAboveValid;
				
				bool valid;
			} Report;
			
			Report getReport(recti rect, ItemType type);
		};
	}
}


#endif
