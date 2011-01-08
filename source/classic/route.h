#ifndef OSS_ROUTE_H
#define OSS_ROUTE_H


#include "../general.h"
#include "../core/core.h"
#include "transportitem.h"


namespace OSS {
	class Tower;
	
	class Route : public CoreObject {
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
		typedef std::vector<Node> Nodes;
		
		/**
		 * Route
		 */
		
		const Pointer<Tower> tower;
		recti origin;
		recti destination;
		
		//Initialization
		Route(Tower * tower);
		Route(const Route & route);
		std::string description();
		
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
	};
}


#endif
