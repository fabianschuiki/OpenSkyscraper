/*
 *  sprite.cpp
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 24.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#include "sprite.h"

using namespace OSS;

void Sprite::updateWorldBounds()
{
	worldBounds.origin = worldOrigin;
	worldBounds.size = size;
}
