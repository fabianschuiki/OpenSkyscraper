#include "structure.h"

#include "item.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

TowerStructure::TowerStructure(Tower * tower) : Engine::Object(), tower(tower)
{
}
