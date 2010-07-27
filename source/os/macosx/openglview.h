#import <Cocoa/Cocoa.h>
#import "pointer.h"
#import "engine.h"


@interface OpenSkyscraperOpenGLView : NSOpenGLView
{
	NSPoint previousMouseLocationInWindow;
	OSS::Pointer<OSS::Engine> engine;
}

@end
