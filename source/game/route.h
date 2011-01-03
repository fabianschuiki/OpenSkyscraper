#ifndef OSS_ROUTE_H
#define OSS_ROUTE_H


#include "../general.h"
#include "../core/core.h"
#include "transportitem.h"


namespace OSS {
	class Route : public CoreObject {
	public:
		class Node {
			recti start;
			Pointer<TransportItem> transport;
			recti end;
		};
		
		std::queue<Node> nodes;
	};
}


#endif
