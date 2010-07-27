#import "openglview.h"
#import "application.h"

using namespace OSS;


@interface OpenSkyscraperOpenGLView ()

- (void)sendEvent:(Event *)event;

@end


@implementation OpenSkyscraperOpenGLView

- (void)awakeFromNib
{
	[NSTimer scheduledTimerWithTimeInterval:1.0/60 target:self selector:@selector(advance) userInfo:nil repeats:YES];
}

- (void)dealloc
{
	engine = NULL;
	[super dealloc];
}

- (void)prepareOpenGL
{
	[super prepareOpenGL];
	engine = new Engine;
	//engine->renderer->prepareOpenGL();
}

- (void)advance
{
	//Simulate mouse moved events
	NSPoint mouseLocation = [[self window] mouseLocationOutsideOfEventStream];
	if (!NSEqualPoints(mouseLocation, previousMouseLocationInWindow)) {
		Event e;
		e.typeMask = Event::mouseMoved;
		e.position = double2(mouseLocation.x, mouseLocation.y);
		[self sendEvent:&e];
		previousMouseLocationInWindow = mouseLocation;
	}
	
	[self display];
}

- (void)drawRect:(NSRect)dirtyRect
{
	engine->renderer->draw();
	[[self openGLContext] flushBuffer];
}

- (void)reshape
{
	[super reshape];
	int2 size(NSWidth([self bounds]), NSHeight([self bounds]));
	engine->renderer->reshape(size);
}

- (void)sendEvent:(Event *)event
{
	engine->input->handleEvent(event);
}

- (void)handleMouseEvent:(NSEvent *)theEvent
{
	Event e;
	e.button = [theEvent buttonNumber];
	if ((1 << [theEvent type]) & (NSLeftMouseDownMask | NSRightMouseDownMask | NSOtherMouseDownMask))
		e.typeMask = Event::mouseDown;
	if ((1 << [theEvent type]) & (NSLeftMouseDraggedMask | NSRightMouseDraggedMask | NSOtherMouseDraggedMask))
		e.typeMask = Event::mouseDragged;
	if ((1 << [theEvent type]) & (NSLeftMouseUpMask | NSRightMouseUpMask | NSOtherMouseUpMask))
		e.typeMask = Event::mouseUp;
	
	NSPoint location = [theEvent locationInWindow];
	e.position = double2(location.x, /*NSHeight([self bounds]) - */location.y);
	[self sendEvent:&e];
}

- (void)mouseDown:(NSEvent *)theEvent
{
	[self handleMouseEvent:theEvent];
}
- (void)mouseDragged:(NSEvent *)theEvent
{
	[self handleMouseEvent:theEvent];
}
- (void)mouseUp:(NSEvent *)theEvent
{
	[self handleMouseEvent:theEvent];
}

- (void)rightMouseDown:(NSEvent *)theEvent
{
	[self handleMouseEvent:theEvent];
}
- (void)rightMouseDragged:(NSEvent *)theEvent
{
	[self handleMouseEvent:theEvent];
}
- (void)rightMouseUp:(NSEvent *)theEvent
{
	[self handleMouseEvent:theEvent];
}

@end
