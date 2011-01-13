#include "escalator.h"

using namespace OSS;
using namespace Classic;


ItemDescriptor EscalatorItem::descriptor = {
	kEscalatorType,
	kStructureGroup,
	kTransportCategory,
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
