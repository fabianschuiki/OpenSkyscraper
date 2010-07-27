/*
 *  lobbyitem.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 26.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef __OSS_LOBBYITEM_H__
#define __OSS_LOBBYITEM_H__
#include "item.h"

namespace OSS {
	class LobbyItem : public Item {
	public:		
		LobbyItem();
		
		//void draw();
		virtual const GLfloat * debugGLColor() { static const GLfloat c[3] = {1, 0, 0}; return c; }
	};
}

#endif
