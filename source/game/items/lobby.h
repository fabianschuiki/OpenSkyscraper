#ifndef OSS_LOBBY_H
#define OSS_LOBBY_H

#include "../../general.h"
#include "../item.h"


namespace OSS {
	class LobbyItem : public Item {
	public:
		static Item::Descriptor descriptor;
		
		//Initialization
	public:
		LobbyItem(Tower * tower);
		
		void init();
		void update();
		
		//Entrances
	private:
		Sprite entrances[2];
	public:
		void initEntrances();
		void updateEntrances();
		
		//Basic Sprites
		void initCeiling();
		void initBackground();
		
		//Drawing
		void draw(rectd visibleRect);
		
		//Notifications
		void onChangeLocation();
	};
}


#endif
