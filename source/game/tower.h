#ifndef OSS_TOWER_H
#define OSS_TOWER_H

#include "../general.h"
#include "../base/base.h"
#include "../core/core.h"
#include "item.h"
#include "route.h"


namespace OSS {
	class Tower : public CoreObject {
	public:
		/**
		 * Initialization
		 */
		
		//Constructor
		Tower();
		
		//Subinitializations
		void initBackground();
		void initEnvironment();
		void initConstruction();
		
		
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
		void advanceTime(double dt);
		void advanceFacilities(double dt);
		void advanceTransport(double dt);
		void advanceBackground(double dt);
		
		
		/**
		 * Background
		 */
		Pointer<Sprite> groundSprite;
		Pointer<Sprite> skySprites[10][2];
		Pointer<Sprite> citySprite;
		Pointer<Sprite> craneSprite;
		void renderBackground(rectd visibleRect);
		
		//Sky State
		typedef enum {
			kDayState = 1,
			kTwilightState,
			kNightState,
			kOvercastState,
			kRainState
		} SkyState;
		SkyState skyState[2];
		bool isRainyDay;
		double rainAnimationTime;
		unsigned int rainIndex;
		double nextThunderCountdown;
		void setSkyState(SkyState state);
		void setSkyState(SkyState current, SkyState target, double interpolation);
		void updateSkySpriteTextures(unsigned int stateIndex);
		
		
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
		bool constructFlexibleWidthItem(Item::Descriptor * descriptor, recti currentRect, recti previousRect);
		bool constructItem(Item::Descriptor * descriptor, recti rect);
		bool checkIfRectMeetsDescriptorRequirements(Item::Descriptor * descriptor, recti rect);
		
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
		
		void insertNewItem(Item::Descriptor * descriptor, recti rect);
		void eraseItem(Item * item);
		void eraseItem(unsigned int itemID);
		void eraseItem(unsigned int itemID, Item * item);
		
		
		/**
		 * Sound Effects
		 */
		SoundEffect constructionSoundFlexible;
		SoundEffect fundsTransferSound;
		
		SoundEffect birdsMorningSound;
		SoundEffect rainSound;
		
		
		/**
		 * Environment
		 */
		
		//Time
		double time;
		unsigned int date;
		unsigned int getDayOfWeek();	//0,1 = WD; 2 = WE
		unsigned int getQuarter();		//0..3
		unsigned int getYear();			//0..inf
		bool isWeekday();
		bool isWeekend();
		
		//Rating
		unsigned short rating;
		
		//Funds
		long funds;
		void transferFunds(long amount);
		
		//Population
		int population;
		void recalculatePopulation();
		
		
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
	public:
		bool checkTime(double previousTime, double alarmTime);
		bool checkTime(double alarmTime);
		
		
		/**
		 * Pathfinder
		 */
		Route * findRoute(recti origin, recti destination);
	private:
		typedef struct {
			unsigned int elevatorsUsed;
			unsigned int stairsUsed;
			unsigned int escalatorsUsed;
		} PathfinderStats;
		bool findRoute(recti origin, recti destination, TransportItem * transport,
					   ItemSet usedTransports, PathfinderStats stats, Route * route);
		
		
		/**
		 * Uncategorized
		 */
	};
}


#endif
