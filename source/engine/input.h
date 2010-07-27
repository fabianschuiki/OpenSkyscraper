/*
 *  input.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 24.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef __OSS_INPUT_H__
#define __OSS_INPUT_H__
#include "common.h"
#include "subsystem.h"
#include "event.h"

namespace OSS {
	class Input : public Subsystem {
	public:
		Input(Engine * engine) : Subsystem(engine) {}
		
		//Mouse events
		void handleEvent(Event * event);
	};
}

#endif
