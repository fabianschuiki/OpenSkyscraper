#ifndef OSS_LOBBY_H
#define OSS_LOBBY_H

#include "../../general.h"
#include "../item.h"


namespace OSS {
	class LobbyItem : public Item {
	public:
		static Item::Descriptor descriptor;
		
		//Decoration Sprites
		Pointer<Sprite> entranceSprites[2];
		
		//Rendering
		virtual void draw(rectd visibleRect);
		
		//Notifications
		virtual void onPrepare();
	};
}


#endif
