/*
 *  engine-macosx.mm
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 25.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#import <Cocoa/Cocoa.h>
#include "engine.h"

std::string OSS::Engine::pathForResource(std::string type, std::string name)
{
	std::string path = [[[NSBundle mainBundle] resourcePath] UTF8String];
	path += "/";
	if (type.length() > 0) {
		path += type;
		path += "/";
	}
	path += name;
	return path;
}
