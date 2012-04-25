#pragma once
#include "../Item.h"

namespace OT {
	namespace Item {
		namespace Elevator {
			class Car;
			class Queue;
			
			class Elevator : public Item
			{
			public:
				Elevator(Game * game, AbstractPrototype * prototype) : Item(game, prototype) {}
				virtual ~Elevator();
				
				virtual void init();
				
				std::string shaftBitmap;
				std::string carBitmap;
				double maxCarAcceleration;
				double maxCarSpeed;
				unsigned int maxCarCapacity;
				
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
				
				typedef std::set<Car *> Cars;
				Cars cars;
				void clearCars();
				void addCar(int floor);
				
				typedef enum {
					kUp   = 1,
					kNone = 0,
					kDown = -1
				} Direction;
				
				virtual bool canHaulPeople() const { return true; }
				virtual bool connectsFloor(int floor) const;
				virtual bool isElevator() const { return true; }
				
				virtual void addPerson(Person * p);
				virtual void removePerson(Person * p);
				
				typedef std::set<Queue *> Queues;
				Queues queues;
				Queue * getQueue(int floor, Direction dir);
				void cleanQueues();
				
				void called(Queue * queue);
				void respondToCalls();
				Queue * getMostUrgentQueue();
				Car * getIdleCar(int floor);
				
				void decideCarDestination(Car * car);
			};
		}
	}
}
