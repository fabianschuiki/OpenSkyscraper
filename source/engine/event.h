/*
 *  event.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 24.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef __OSS_EVENT_H__
#define __OSS_EVENT_H__
#include "common.h"

namespace OSS {
	class Event {
	public:
		enum {
			//Mouse Interaction
			mouseDown		= (1 << 0),
			mouseDragged	= (1 << 1),
			mouseUp			= (1 << 2),
			mouse			= (mouseDown | mouseDragged | mouseUp),
			
			//Mouse movement
			mouseMoved = (1 << 3)
		};
		unsigned int typeMask;
		double2 position;
		
		//Mouse Events
		unsigned short button;
	};
}

#endif
