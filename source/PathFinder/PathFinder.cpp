#include <cmath>
#include "../Item/Item.h"
#include "../Route.h"
#include "MapNode.h"
#include "PathFinder.h"
#include "../Logger.h"
using namespace OT;


PathFinder::PathFinder() {}

PathFinder::~PathFinder() { clear(); }

Route PathFinder::findRoute(const MapNode *start_mapnode, const MapNode *end_mapnode, Item::Item *start_item, Item::Item *end_item, bool serviceRoute) {
	MapNode::Point start_point(start_item->position.x + start_item->size.x/2, start_mapnode->position.y);
	MapNode::Point end_point(end_item->position.x + end_item->size.x/2, end_mapnode->position.y);

	MapSearchNode nodeStart(start_mapnode);
	nodeStart.parent_item = start_item;
	nodeStart.start_point = start_point;
	nodeStart.end_point = end_point;
	nodeStart.serviceRoute = serviceRoute;
	MapSearchNode nodeEnd(end_mapnode);
	astarsearch.SetStartAndGoalStates(nodeStart, nodeEnd);

	unsigned int SearchSteps = 0;
	do {
		SearchState = astarsearch.SearchStep();
		SearchSteps++;
	} while(SearchState == AStarSearch< MapSearchNode >::SEARCH_STATE_SEARCHING);

	Route r;
	buildRoute(r, start_item, end_item);
	clear();

	return r;
}


void PathFinder::clear() {
	if(SearchState == AStarSearch< MapSearchNode >::SEARCH_STATE_SUCCEEDED) astarsearch.FreeSolutionNodes();
	astarsearch.EnsureMemoryFreed();
}

void PathFinder::buildRoute(Route &r, Item::Item *start_item, Item::Item *end_item) {
	if(SearchState != AStarSearch< MapSearchNode >::SEARCH_STATE_SUCCEEDED) {
		r.clear();
		return;
	}

	if(!start_item || !end_item) {
		r.clear();
		return;
	}

	MapSearchNode *end_node = astarsearch.GetSolutionEnd();
	MapSearchNode *start_node = astarsearch.GetSolutionStart();

	if(start_node->IsSameState(*end_node)) {
		r.add(start_item, start_node->mapNode->position.y);
		r.add(end_item, end_node->mapNode->position.y);
		return;
	}
	
	MapSearchNode *n = start_node;
	MapSearchNode *n_child;
	r.add(start_item, n->mapNode->position.y);
	n = astarsearch.GetSolutionNext();
	while(n != end_node) {
		n_child = astarsearch.GetSolutionNext();
		Item::Item *i = n_child->parent_item;
		if(i) {
			int toFloor;
			if(i->canHaulPeople()) toFloor = n_child->mapNode->position.y;
			else				   toFloor = n->mapNode->position.y;

			Route::Node &rn_prev = r.nodes.back();
			if(i->isElevator() && rn_prev.item->isElevator() && i == rn_prev.item) {
				// Moving along same elevator
				rn_prev.toFloor = toFloor;
			} else r.add(i, toFloor);
		}
		n = n_child;
	};
	r.add(end_item, n->mapNode->position.y);
	r.updateScore((int)std::abs(end_node->g + end_node->h));
}
