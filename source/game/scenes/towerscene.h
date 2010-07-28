#ifndef OSS_TOWERSCENE_H
#define OSS_TOWERSCENE_H

#include "../../general.h"
#include "../../core/scene.h"
#include "../tower.h"


namespace OSS {
	class TowerScene : public Scene {
	public:
		//Point of interest, i.e. the point in the world that's centered on-screen
		double2 POI;
		
		//Calculated culling values
		rectd visibleRect;
		recti visibleCells;
		
		//Visible items
		std::set<unsigned int> visibleFacilities;
		std::set<unsigned int> visibleTransports;
		
		//The tower currently being played
		Pointer<Tower> tower;
		
		
		//Simulation
		void advance(double dt);
		
		//Rendering
		void render();
		void renderFacilities();
		void renderTransports();
		
		//OpenGL State
		void onMoveOnScreen();
		void onMoveOffScreen();
	};
}


#endif
