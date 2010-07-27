/*
 *  input.cpp
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 24.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#include "engine.h"
#include "input.h"

using namespace OSS;

void Input::handleEvent(Event * event)
{
	engine->currentRootNode()->handleEvent(event);
	/*if (!handled)
		std::cout << this->description() << " not handled" << std::endl;*/
}
