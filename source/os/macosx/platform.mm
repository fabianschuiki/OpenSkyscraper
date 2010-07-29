#include "../../general.h"
#include "../../core/core.h"
#import <Cocoa/Cocoa.h>

using namespace OSS;


std::string Platform::resourcesPath()
{
	return [[[NSBundle mainBundle] resourcePath] UTF8String];
}
