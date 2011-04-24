#include "officeworker.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

OfficeWorker::OfficeWorker(Tower * tower, OfficeItem * office) : Person(tower), office(office)
{
}

