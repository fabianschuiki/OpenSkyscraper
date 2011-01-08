#include "route.h"

#include "tower.h"
#include "../items/transports/transport.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Node
//----------------------------------------------------------------------------------------------------

Route::Node::Node(Route * route, recti start, TransportItem * transport, recti end)
{
	this->route = route;
	this->start = start;
	this->transport = transport;
	this->end = end;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

Route::Route(Tower * tower) : tower(tower), Core::Object()
{
}

Route::Route(const Route & route) : tower(route.tower), Core::Object()
{
	origin = route.origin;
	destination = route.destination;
	nodes = route.nodes;
}

std::string Route::description()
{
	std::string r = "route (\n";
	r += "\t"; r += origin.description() + "\n";
	for (Nodes::iterator node = nodes.begin(); node != nodes.end(); node++) {
		r += "\t";
		r += (*node).start.description();
		r += " >-- ";
		r += (*node).transport->description();
		r += " --> ";
		r += (*node).end.description();
		r += "\n";
	}
	r += "\t"; r += destination.description() + "\n";
	r += ")";
	return r;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Nodes
//----------------------------------------------------------------------------------------------------

const Route::Nodes & Route::getNodes() const
{
	return nodes;
}

void Route::addNode(const Node & node)
{
	nodes.push_back(node);
}

void Route::addNode(recti start, TransportItem * transport, recti end)
{
	nodes.push_back(Node(this, start, transport, end));
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Distance
//----------------------------------------------------------------------------------------------------

const unsigned int Route::getDistance()
{
	return 100;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Operators
//----------------------------------------------------------------------------------------------------

Route & Route::operator= (const Route & route)
{
	assert(route.tower == tower);
	origin = route.origin;
	destination = route.destination;
	nodes = route.nodes;
	return *this;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Validation
//----------------------------------------------------------------------------------------------------

bool Route::isValid()
{
	for (Nodes::iterator node = nodes.begin(); node != nodes.end(); node++) {
		if (!(*node).transport->isValid())
			return false;
		if (!(*node).transport->connectsToFloor((*node).start.minY()))
			return false;
		if (!(*node).transport->connectsToFloor((*node).end.minY()))
			return false;
	}
	return true;
}
