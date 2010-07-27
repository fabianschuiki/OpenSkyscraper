/*
 *  gameui.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 24.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef __OSS_GAMEUI_H__
#define __OSS_GAMEUI_H__
#include "common.h"
#include "node.h"

namespace OSS {
	class GameUI : public Node {
	public:
		GameUI();
		
		void rebuild();
	};
}

#endif
