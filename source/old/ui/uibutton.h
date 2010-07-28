/*
 *  uibutton.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 24.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef __OSS_UIBUTTON_H__
#define __OSS_UIBUTTON_H__
#include "common.h"
#include "opengl.h"
#include "sprite.h"
#include "texture.h"
#include "engine.h"

namespace OSS {
	class UIButton : public Sprite {
	public:
		Pointer<Texture> openttd;
		
		UIButton() {
			openttd = new Texture(Engine::pathForResource("", "openttd.png"));
		}
		
		void draw();
		
		bool onMouseDown(double2 position, unsigned short button);
	};
}

#endif
