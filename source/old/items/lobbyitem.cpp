/*
 *  lobbyitem.cpp
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 26.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#include "lobbyitem.h"
#include "tower.h"

using namespace OSS;


LobbyItem::LobbyItem() : Item()
{
	isCollapsible = true;
	minSize = int2(4, 1);
	frame.size = minSize;
}

/*void LobbyItem::draw()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3f(1, 0, 0);
	Geometry::drawRect(rectd(worldOrigin, worldSize));
}*/
