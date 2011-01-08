#ifndef OSS_LOBBY_H
#define OSS_LOBBY_H

#include "../../general.h"
#include "../facilityitem.h"


namespace OSS {
	class LobbyItem : public FacilityItem {
	public:
		static Item::Descriptor descriptor;
		
		//Initialization
	public:
		LobbyItem(Tower * tower);
		
		void init();
		void update();
		
		std::string getLobbyTextureBaseName();
		
		//Entrances
	private:
		Sprite outsideEntrances[2];
		Sprite insideEntrance;
	public:
		void initEntrances();
		void updateEntrances();
		
		//Basic Sprites
		void initBackground();
		void updateBackground();
		
		//Drawing
		void draw(rectd visibleRect);
		
		//Notifications
		void onChangeLocation();
	};
}


#endif
