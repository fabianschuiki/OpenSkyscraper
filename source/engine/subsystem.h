/*
 *  subsystem.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 24.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef __OSS_SUBSYSTEM_H__
#define __OSS_SUBSYSTEM_H__
#include "object.h"

namespace OSS {
	/**
	 * Forward declaration of the engine class since it is required
	 * here but we want to prevent loop references.
	 */
	class Engine;
	
	/**
	 * Engine subsystem. Simple abstract class that keeps a reference
	 * to the engine to provide singleton-free code.
	 */
	class Subsystem : public Object {
	public:
		Engine * engine;
		Subsystem(Engine * engine) : engine(engine) {}
	};
}

#endif
