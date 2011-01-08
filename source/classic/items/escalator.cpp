#include "escalator.h"
#include "../tower.h"

using namespace OSS;


Item::Descriptor EscalatorItem::descriptor = {
	Item::kEscalatorType,
	Item::kStructureGroup,
	Item::kTransportCategory,
	1,
	(0),
	20000,
	int2(8, 2),
	int2(8, 2),
	rectmaski(new recti(0, 0, 4, 1), new recti(4, 1, 4, 1), NULL)
};





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

EscalatorItem::EscalatorItem(Tower * tower) : StairslikeItem(tower, &descriptor)
{
	numAnimationFrames = 8;
	numAnimationFramesPerTexture = 8;
	baseTextureName = "simtower/transport/escalator";
}
