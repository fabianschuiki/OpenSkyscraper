#include "scheduledperson.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Schedule
//----------------------------------------------------------------------------------------------------

const Schedule::Nodes & Schedule::getNodes() const
{
	return nodes;
}

void Schedule::addNode(Node & node)
{
	Nodes::iterator n;
	for (n = nodes.begin(); n != nodes.end(); n++)
		if ((*n).time > node.time)
			break;
	nodes.insert(n, node);
}

void Schedule::addNode(double time, Item * destination)
{
	Node n;
	n.time = time;
	n.destination = destination;
	addNode(n);
}

string Schedule::description()
{
	stringstream s;
	s << "schedule (\n";
	for (Nodes::iterator n = nodes.begin(); n != nodes.end(); n++) {
		s << "\t" << (*n).time << "\t -> ";
		if ((*n).destination)
			s << (*n).destination->description();
		else
			s << "[out]";
	}
	s << ")";
	return s.str();
}

Schedule::Node * Schedule::getNodeForTime(double time)
{
	for (Nodes::reverse_iterator n = nodes.rbegin(); n != nodes.rend(); n++)
		if ((*n).time < time)
			return &(*n);
	return NULL;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Scheduled Person
//----------------------------------------------------------------------------------------------------

Schedule * ScheduledPerson::getSchedule() const
{
	return schedule;
}

void ScheduledPerson::setSchedule(Schedule * schedule)
{
	if (this->schedule != schedule) {
		this->schedule = schedule;
		initSchedule();
		updateFromSchedule();
	}
}

void ScheduledPerson::initSchedule()
{
	previousNode = NULL;
	if (schedule)
		OSSObjectLog << "schedule set to " << schedule->description() << std::endl;
}

void ScheduledPerson::updateFromSchedule()
{
	if (!getSchedule())
		return;
	
	Schedule::Node * node = getSchedule()->getNodeForTime(tower->time->getTime());
	if (node != previousNode) {
		previousNode = node;
		setDestination(node ? node->destination : NULL);
	}
}
