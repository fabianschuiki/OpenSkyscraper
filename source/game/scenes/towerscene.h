#ifndef TOWERSCENE_H
#define TOWERSCENE_H

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
		
		//The tower currently being played
		Pointer<Tower> tower;
		
		
		//Rendering
		void render();
		
		//OpenGL State
		void onMoveOnScreen();
		void onMoveOffScreen();
	};
}


#endif
