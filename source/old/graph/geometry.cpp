/*
 *  geometry.cpp
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 26.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#include "geometry.h"

using namespace OSS;


void Geometry::drawRect(rectd rect) {
	glBegin(GL_QUADS);
	glVertex2f(rect.minX(), rect.minY());
	glVertex2f(rect.maxX(), rect.minY());
	glVertex2f(rect.maxX(), rect.maxY());
	glVertex2f(rect.minX(), rect.maxY());
	glEnd();
}

void Geometry::drawTexturedRect(rectd rect, rectd texrect) {
	glBegin(GL_QUADS);
	glTexCoord2f(texrect.minX(), texrect.minY()); glVertex2f(rect.minX(), rect.minY());
	glTexCoord2f(texrect.maxX(), texrect.minY()); glVertex2f(rect.maxX(), rect.minY());
	glTexCoord2f(texrect.maxX(), texrect.maxY()); glVertex2f(rect.maxX(), rect.maxY());
	glTexCoord2f(texrect.minX(), texrect.maxY()); glVertex2f(rect.minX(), rect.maxY());
	glEnd();
}

void Geometry::drawFrameRect(rectd rect) {
	glBegin(GL_LINE_STRIP);
	glVertex2f(rect.minX() + 0.5, rect.minY() + 0.5);
	glVertex2f(rect.maxX() - 0.5, rect.minY() + 0.5);
	glVertex2f(rect.maxX() - 0.5, rect.maxY() - 0.5);
	glVertex2f(rect.minX() + 0.5, rect.maxY() - 0.5);
	glVertex2f(rect.minX() + 0.5, rect.minY() + 0.5);
	glEnd();
}
