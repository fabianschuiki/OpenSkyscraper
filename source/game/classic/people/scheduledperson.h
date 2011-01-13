#ifndef OSS_SCHEDULEDPERSON_H
#define OSS_SCHEDULEDPERSON_H

#include "person.h"


namespace OSS {
	namespace Classic {
		/**
		 * Schedule
		 *
		 * Class used in ScheduledPerson to organize the person's day.
		 */
		class Schedule : public Object {
		public:
			//Schedule Node
			class Node {
			public:
				double time;
				Pointer<Item> destination;
			};
			
			//Ordered list of nodes
		private:
			typedef list<Node> Nodes;
			Nodes nodes;
		public:
			const Nodes & getNodes() const;
			void addNode(Node & node);
			void addNode(double time, Item * destination);
			string description();
			
			//Obtaining nodes
			Node * getNodeForTime(double time);
		};
		
		/**
		 * ScheduledPerson
		 */
		class ScheduledPerson : public Person {
		public:
			//Initialization
			ScheduledPerson(Tower * tower) : Person(tower) {}
			
			//Schedule
		private:
			Pointer<Schedule> schedule;
			Schedule::Node * previousNode;
		public:
			Schedule * getSchedule() const;
			void setSchedule(Schedule * schedule);
			void initSchedule();
			void updateFromSchedule();
		};
	}
}


#endif
