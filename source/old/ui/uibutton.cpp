/*
 *  uibutton.cpp
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 24.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#include "uibutton.h"

using namespace OSS;

void UIButton::draw()
{
	Sprite::draw();
	glBindTexture(GL_TEXTURE_2D, openttd->texture);
	glColor3f(1, 1, 1);
	drawTexturedRect(rectd(worldOrigin, size), rectd(0, 0, 1, 1));
}

bool UIButton::onMouseDown(double2 position, unsigned short button)
{
	std::cout << "clicked " << this->description() << " at " << position.x << ", " << position.y << " with button " << button << std::endl;
	return true;
}
