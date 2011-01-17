#ifndef OSS_GAME_GAMEOBJECT_H
#define OSS_GAME_GAMEOBJECT_H

#include "external.h"


namespace OSS {
	namespace Classic {
		class GameObject : public SceneObject, public Classic::Responder {
		public:
			GameObject() : Classic::Responder(this) {
				advanceTime = 0;
			}
			
			//The type name should be some string identifying this object. It may be used for
			//example in implementations of Item where you have to distinguish between different
			//kinds of people moving to and from your item.
			virtual string getTypeName() { return ""; }
			
			
			/**
			 * Simulation
			 */
		private:
			map<string, double> advanceTimestamps;
			double advanceTime;
			
		public:
			virtual void advance(double dt) {
				
				//Advance the overall time value
				advanceTime += dt;
				
				SceneObject::advance(dt);
			}
			
			bool shouldAdvance(string identifier, double period) {
				
				//If there's no timestamp for this identifier, create one and set it to 0.
				if (!advanceTimestamps.count(identifier))
					advanceTimestamps[identifier] = 0;
				
				//If the difference between the advance time and the timestamp for this identifier exceeds the
				//period, return true.
				if (advanceTime - advanceTimestamps[identifier] >= period) {
					advanceTimestamps[identifier] = advanceTime;
					return true;
				}
				
				return false;
			}
		};
	}
}


#endif
