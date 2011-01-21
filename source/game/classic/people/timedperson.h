#ifndef OSS_TIMEDPERSON_H
#define OSS_TIMEDPERSON_H

#include "person.h"


namespace OSS {
	namespace Classic {
		class TimedPerson : public Person {
			
			/**
			 * Initialization
			 */
		public:
			//Initialization
			TimedPerson(Tower * tower);
			
			
			/**
			 * Next Destination
			 */
		private:
			Pointer<Item> nextDestination;
			double nextDestinationTime;
			bool nextDestinationValid;
			double nextDestinationPauseDuration;
		public:
			
			//Information on the next destination
			Item * getNextDestination();
			double getNextDestinationTime();
			double getNextDestinationPauseDuration();
			bool isNextDestinationValid();
			
			//Changing the next destination
			void setNextDestination(Item * destination);
			void setNextDestination(Item * destination, double duration);
			void setNextDestination(double time, Item * destination);
			void setNextDestination(double time, Item * destination, double duration);
			void clearNextDestination();
			
			
			/**
			 * Location
			 */
		private:
			double pauseDurationAtDestination;
			double pauseDuration;
			double pauseEndTime;
		public:
			virtual void didMoveToItem();
			
			//Pause duration once arrived at the destination
			double getPauseDurationAtDestination();
			void setPauseDurationAtDestination(double duration);
			
			//Pause duration
			double getPauseDuration();
			void setPauseDuration(double duration);
			
			//End of pause
			double getPauseEndTime();
			void setPauseEndTime(double time);
			void setPauseEndTimeDaily(double time);
			void setPauseEndTimeDailyFuture(double time); //ensures that the pause end time lies in the future
			
			//Convenience
			bool isPausing();
			bool hasNoPlans();
			
			
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
			virtual void updateNextDestination();
		};
	}
}


#endif
