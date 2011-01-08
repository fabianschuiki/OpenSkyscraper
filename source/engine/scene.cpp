#include "scene.h"

using namespace OSS;
using namespace Engine;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

Scene::Scene(EngineCore * engine) : Responder(), engine(engine)
{
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Presentation
//----------------------------------------------------------------------------------------------------

bool Scene::isOnScreen()
{
	return (engine->getScene() == this);
}
