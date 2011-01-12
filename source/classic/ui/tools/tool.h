#ifndef OSS_CLASSIC_UI_TOOLS_TOOL_H
#define OSS_CLASSIC_UI_TOOLS_TOOL_H

#include "../../external.h"
#include "../../responder.h"


namespace OSS {
	namespace Classic {
		class ToolsUI;
		
		class Tool : public Responder {
			
			/**
			 * Construction
			 */
		public:
			const Pointer<ToolsUI> ui;
			Tool(ToolsUI * ui) : Responder(), ui(ui) {}
			
			
			/**
			 * State
			 */
		public:
			virtual void update() {}
			
			
			/**
			 * Drawing
			 */
		public:
			virtual void draw(rectd dirtyRect) {}
			
			
			/**
			 * Event Handling
			 */
		public:
			virtual bool eventMouseDown(Core::MouseButtonEvent * event) { return false; }
			virtual bool eventMouseUp(Core::MouseButtonEvent * event) { return false; }
			virtual bool eventMouseMoved(Core::MouseMovedEvent * event) { return false;}
		};
	}
}


#endif
