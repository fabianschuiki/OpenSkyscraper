#include "lobbyitem.h"

using namespace OSS;


Item::Descriptor LobbyItem::descriptor = {
	Item::kLobbyType,
	Item::kStructureGroup,
	Item::kFacilityCategory,
	1,
	(Item::kFlexibleWidthAttribute | Item::kEvery15thFloorAttribute | Item::kNotBelowGroundAttribute),
	5000, 
	int2(1, 1),
	int2(4, 1)
};
