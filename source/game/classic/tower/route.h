#ifndef OSS_CLASSIC_TOWER_ROUTE_H
#define OSS_CLASSIC_TOWER_ROUTE_H

#include "../game.h"


namespace OSS {
	namespace Classic {
		class Tower;
		class TransportItem;
		
		class Route : public Object {
		public:
			/**
			 * Route Node
			 */
			class Node {
			private:
				Route * route;
				
			public:
				recti start;
				Pointer<TransportItem> transport;
				recti end;
				
				Node(Route * route, recti start, TransportItem * transport, recti end);
			};
			typedef vector<Node> Nodes;
			
			/**
			 * Route
			 */
			const Pointer<Tower> tower;
			recti origin;
			recti destination;
			
			//Initialization
			Route(Tower * tower);
			Route(const Route & route);
			string description();
			
			//Nodes
		private:
			Nodes nodes;
		public:
			const Nodes & getNodes() const;
			void addNode(const Node & node);
			void addNode(recti start, TransportItem * transport, recti end);
			
			//Distance
			const unsigned int getDistance();
			
			//Operators
			Route & operator= (const Route & route);
			
			//Validation
			bool isValid();
			
			
			/**
			 * Pathfinder
			 */
		private:
			typedef struct {
				unsigned int elevatorsUsed;
				unsigned int stairsUsed;
				unsigned int escalatorsUsed;
			} PathfinderStats;
			typedef std::set<TransportItem *> UsedTransportsSet;
			static bool findRoute(Tower * tower, recti origin, recti destination,
								  TransportItem * transport,
								  UsedTransportsSet usedTransports, PathfinderStats stats,
								  Route * route);
			
		public:
			static Route * findRoute(Tower * tower, recti origin, recti destination);
		};
	}
}


#include "tower.h"
#include "../items/transports/transport.h"


#endif
