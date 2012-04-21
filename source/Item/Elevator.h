#pragma once
#include "ElevatorQueue.h"
#include "Item.h"

namespace OT {
	namespace Item {
		class ElevatorCar;
		
		class Elevator : public Item
		{
		public:
			Elevator(Game * game, OT::Item::AbstractPrototype * prototype) : Item(game, prototype) {}
			virtual ~Elevator();
			
			virtual void init();
			
			std::string shaftBitmap;
			std::string carBitmap;
			int maxPassengers;
			
			double animation;
			int frame;
			
			Sprite shaft;
			Sprite topMotor;
			Sprite bottomMotor;
			
			void updateSprite();
			virtual void Render(sf::RenderTarget & target) const;
			virtual void advance(double dt);
			
			virtual void encodeXML(tinyxml2::XMLPrinter& xml);
			virtual void decodeXML(tinyxml2::XMLElement& xml);
			
			virtual rectd getMouseRegion();
			void repositionMotor(int motor, int y);
			std::set<int> unservicedFloors;
			
			typedef std::set<ElevatorCar *> Cars;
			Cars cars;
			void clearCars();
			void addCar(int floor);
			
			typedef enum {
				kNone = -1,
				kUp   = 0,
				kDown = 1
			} Direction;
			
			virtual bool canHaulPeople() const { return true; }
			virtual bool connectsFloor(int floor) const;
			virtual bool isElevator() const { return true; }
			
			virtual void addPerson(Person * p);
			virtual void removePerson(Person * p);
			
			typedef struct { ElevatorQueue * dirs[2]; } QueuePair;
			typedef std::map<int, QueuePair> Queues;
			Queues queues;
			ElevatorQueue * getQueue(int floor, Direction dir);
			void cleanQueues();
		};
	}
}
