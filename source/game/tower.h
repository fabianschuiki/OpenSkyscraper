#ifndef OSS_TOWER_H
#define OSS_TOWER_H

#include "../general.h"
#include "../base/base.h"
#include "../core/core.h"
#include "item.h"


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
		 * Uncategorized
		 */
	};
}


#endif
