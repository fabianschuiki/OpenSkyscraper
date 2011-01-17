#ifndef OSS_GAME_CLASSIC_TOWER_ENVIRONMENT_H
#define OSS_GAME_CLASSIC_TOWER_ENVIRONMENT_H

#include "tower.h"


namespace OSS {
	namespace Classic {
		class Person;
		
		class TowerEnvironment : public GameObject {
			
			/**
			 * Construction
			 */
		public:
			const Pointer<Tower> tower;
			
			TowerEnvironment(Tower * tower);
			
			
			/**
			 * Rating
			 */
		private:
			unsigned short rating;
			
		public:
			unsigned short getRating();
			void setRating(unsigned short r);
			
			unsigned int getLobbyStyle();
			
			
			/**
			 * Population
			 */
		private:
			unsigned int population;
			
		public:
			unsigned int getPopulation();
			void setPopulation(unsigned int p);
			
			
			/**
			 * People
			 *
			 * The tower has to maintain two lists of people: One which uses memory management and
			 * is responsible for hosting people that are currently not under control of an item but
			 * find themselves inside the tower. Hotel guests after checking out are removed from
			 * their hotel item, but are still inside the tower. The other one doesn't use memory
			 * management and is responsible for advancing and updating people. The intention is
			 * that when a Person is constructed, it adds itself to the list of people to be ad-
			 * vanced and updated. Vice versa upon destruction. If a person's item is set to NULL,
			 * the person removes itself from the memory managed list of people, vice versa
			 * otherwise.
			 * 
			 * This ensures that all people in existence are listed in the unmanaged list. All
			 * people currently inside the tower are listed in the managed list. When an item gives
			 * up control of a person and the person is told to leave the tower, he/she remains in
			 * the managed list until reaching the lobby.
			 *
			 * TODO: Move this to either the structure, or a separate subclass, or rename this class
			 * to something more appropriate.
			 */
		private:
			set< Person * > existingPeople;
			set< Pointer<Person> > peopleInTower;
			
		public:
			void addPerson(Person * p);
			void removePerson(Person * p);
			
			void addPersonToTower(Person * p);
			void removePersonFromTower(Person * p);
			
			
			/**
			 * Simulatipn
			 */
		public:
			virtual void advance(double dt);
			virtual void advancePeople(double dt);
			
			
			/**
			 * State
			 */
		public:
			virtual void update();
			virtual void updatePopulation();
			virtual void updatePeople();
			
			Updatable::Conditional<TowerEnvironment> updatePopulationIfNeeded;
			Updatable::Conditional<TowerEnvironment> updatePeopleIfNeeded;
		};
	}
}


//#include "../people/person.h"


#endif
