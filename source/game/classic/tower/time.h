#ifndef OSS_CLASSIC_TOWER_TIME_H
#define OSS_CLASSIC_TOWER_TIME_H

#include "tower.h"


namespace OSS {
	namespace Classic {		
		class TowerTime : public GameObject {
			
			/**
			 * Construction
			 */
		public:
			const Pointer<Tower> tower;
			
			TowerTime(Tower * tower);
			
			
			/**
			 * Time
			 */
		private:
			double time;
			
		public:
			double getTime();				//0..inf
			void setTime(double t);
			
			double getTimeOfDay();
			bool isAfter(double a);
			bool isBetween(double a, double b);
			bool isBefore(double b);
			
			unsigned int getDate();
			unsigned int getDayOfWeek();	//0,1 = WD; 2 = WE
			unsigned int getQuarter();		//0..3
			unsigned int getYear();			//0..inf
			bool isWeekday();
			bool isWeekend();
			
			
			/**
			 * Triggering
			 */
		private:
			double previousTime;
			double bufferedPreviousTime;
			
		public:
			bool check(double alarmTime);		//alarmTime [0, inf[
			bool checkDaily(double alarmTime);	//alarmTime [0, 24[
			
			
			/**
			 * Simulation
			 */
		private:
			double getTimeSpeed();
			
		public:
			virtual void advance(double dt);
		};
	}
}


#endif
