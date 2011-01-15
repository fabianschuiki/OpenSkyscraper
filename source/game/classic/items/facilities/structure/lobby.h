#ifndef OSS_CLASSIC_ITEMS_FACILITIES_STRUCTURE_LOBBY_H
#define OSS_CLASSIC_ITEMS_FACILITIES_STRUCTURE_LOBBY_H

#include "../facility.h"


namespace OSS {
	namespace Classic {
		class LobbyItem : public FacilityItem {
		public:
			static ItemDescriptor descriptor;
			
			/**
			 * Initialization
			 */
		public:
			LobbyItem(Tower * tower);
			
			
			/**
			 * State
			 */
		public:
			virtual void update();
			virtual void updateBackground();
			virtual void updateEntrances();
			
			Updatable::Conditional<LobbyItem> updateEntrancesIfNeeded;
			
			virtual void didChangeWorldRect();
			
			
			/**
			 * Drawing
			 */
		private:
			Pointer<Sprite> outsideEntrances[2];
			Pointer<Sprite> insideEntrance;
			
		public:
			string getLobbyTextureBaseName();
			
			virtual void draw(rectd dirtyRect);
			virtual void drawBackground(rectd dirtyRect);
		};
	}
}


#endif
