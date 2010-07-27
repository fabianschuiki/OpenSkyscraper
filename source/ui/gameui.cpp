/*
 *  gameui.cpp
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 24.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#include "gameui.h"
#include "uibutton.h"

using namespace OSS;

GameUI::GameUI()
{
	UIButton * button = new UIButton;
	button->localOrigin = double2(50, 50);
	button->size = double2(64, 64);
	attachChild(button);
}
