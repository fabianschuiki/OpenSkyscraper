#include "../../source/core/application.h"
#import <Cocoa/Cocoa.h>

using namespace OSS;


std::vector<std::string> Core::Application::resourcesPaths()
{
	std::vector<std::string> paths;
	paths.push_back([[[NSBundle mainBundle] resourcePath] UTF8String]);
	NSArray * appSupports = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSAllDomainsMask, YES);
	for (NSString * appSupport in appSupports) {
		NSString * path = [appSupport stringByAppendingPathComponent:@"OpenSkyScraper"];
		paths.push_back([path UTF8String]);
	}
	return paths;
}
