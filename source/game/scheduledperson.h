#ifndef OSS_SCHEDULEDPERSON_H
#define OSS_SCHEDULEDPERSON_H


#include "../general.h"
#include "person.h"


namespace OSS {
	/**
	 * Schedule
	 *
	 * Class used in ScheduledPerson to organize the person's day.
	 */
	class Schedule : public CoreObject {
	public:
		//Schedule Node
		class Node {
		public:
			double time;
			Pointer<Item> destination;
		};
		
		//Ordered list of nodes
	private:
		typedef std::list<Node> Nodes;
		Nodes nodes;
	public:
		const Nodes & getNodes() const;
		void addNode(Node & node);
		void addNode(double time, Item * destination);
		std::string description();
		
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


#endif
