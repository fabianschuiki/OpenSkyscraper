#ifndef OSS_CLASSIC_TOWER_TOWER_H
#define OSS_CLASSIC_TOWER_TOWER_H

#include "../external.h"
#include "../responder.h"

#include "background.h"
#include "environment.h"
#include "funds.h"
#include "../items/itemdescriptor.h"
#include "structure.h"
#include "time.h"


namespace OSS {
	namespace Classic {
		class Item;
		
		class Tower : public Responder {
			
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
			 * Event Sending
			 */
		public:
			virtual bool sendEventToNextResponders(Base::Event * event);
			
			
			/**
			 * Items
			 */
			/*Item * getFacility(unsigned int itemID, bool createIfInexistent = false);
			 Item * getTransport(unsigned int itemID, bool createIfInexistent = false);
			 
			 protected:*/
			typedef std::map< unsigned int, Pointer<Item> > ItemIDMap;
			ItemIDMap items;
			ItemIDMap facilityItems;
			ItemIDMap transportItems;
			
			//by floor
			typedef std::set< Pointer<Item> > ItemSet;
			typedef std::map< int, ItemSet > ItemFloorMap;
			ItemFloorMap itemsByFloor;
			ItemFloorMap facilityItemsByFloor;
			ItemFloorMap transportItemsByFloor;
			
			//Convenience access to the ground lobby
			Item * getGroundLobby();
			
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
			
		public:
			std::map<int, std::map<int, Cell> > cells;
		public:
			Cell * getCell(int2 coordinates, bool createIfInexistent = false);
			
			
			/**
			 * Construction
			 */
			bool constructFlexibleWidthItem(ItemDescriptor * descriptor, recti currentRect, recti previousRect);
			bool constructItem(ItemDescriptor * descriptor, recti rect);
			bool checkIfRectMeetsDescriptorRequirements(ItemDescriptor * descriptor, recti rect);
			
			typedef struct {
				unsigned int empty;
				unsigned int floor;
				unsigned int facility;
				unsigned int transport;
			} CellCount;
			typedef struct {
				CellCount all;
				CellCount masked;
				unsigned int unmaskedCells;
				unsigned int facilityCellsBelow;
				unsigned int facilityCellsAbove;
			} CellAnalysis;
			CellAnalysis analyzeCellsInRect(recti rect, rectmaski mask);
			
			void insertNewItem(ItemDescriptor * descriptor, recti rect);
			void eraseItem(Item * item);
			void eraseItem(unsigned int itemID);
			void eraseItem(unsigned int itemID, Item * item);
			
			
			/**
			 * Sound Effects
			 */
			Engine::SoundEffect constructionSoundFlexible;
			Engine::SoundEffect fundsTransferSound;
			
			Engine::SoundEffect birdsMorningSound;
			Engine::SoundEffect rainSound;
			
			
			/**
			 * Environment
			 */
			
			//Time
		private:
			bool dateAdvanced;
		public:
			bool didDateAdvance();
			
			bool paused;
			int debugSpeed;
			
			
			/**
			 * Timer
			 *
			 * The timer is used to check whether a specific time of the day just happened. This method
			 * requires the previous time and the time which you are interested in, and will return true
			 * if the alarm time lies in between the previous and the current time.
			 *
			 * For convenience, the tower also keeps track of the previous time so you may use the
			 * timer without the previous time argument. This requires that you check the timer at the
			 * same frequency as the tower advances, otherwise some events may be lost.
			 */
		private:
			double previousTime;
			double previousTimeBuffer; //required to enable changes to the time outside of advance()
		public:
			bool checkTime(double previousTime, double alarmTime);
			bool checkTime(double alarmTime);
			
			
			/**
			 * Uncategorized
			 */
		};
	}
}


#endif
