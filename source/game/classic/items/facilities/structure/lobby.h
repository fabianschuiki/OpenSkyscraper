#ifndef OSS_CLASSIC_ITEMS_FACILITIES_STRUCTURE_LOBBY_H
#define OSS_CLASSIC_ITEMS_FACILITIES_STRUCTURE_LOBBY_H

#include "../facility.h"


namespace OSS {
	namespace Classic {
		class LobbyItem : public FacilityItem {
		public:
			static ItemDescriptor descriptor;
			
			//Initialization
		public:
			LobbyItem(Tower * tower);
			
			void init();
			void update();
			
			string getLobbyTextureBaseName();
			
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
}


#endif
