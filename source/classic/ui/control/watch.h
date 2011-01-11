#ifndef OSS_CLASSIC_UI_CONTROL_WATCH_H
#define OSS_CLASSIC_UI_CONTROL_WATCH_H

#include "../external.h"


namespace OSS {
	namespace Classic {
		class Watch : public GUI::View {
		public:
			const Pointer<ControlWindow> controlWindow;
			
			Watch(ControlWindow * c) : GUI::View(), controlWindow(c) {}
			
			virtual void draw(rectd dirtyRect)
			{
				//Get the tower from our control window parent
				Tower * tower = controlWindow->tower;
				if (!tower)
					return;
				
				//Calculate the center of the watch
				double2 center = getFrame().center();
				double radius = getFrameSize().x / 2;
				
				//Calculate the rotation angles of the hour and minute hands
				double hourHandAngle = 2 * M_PI * (tower->time / 12);
				double minuteHandAngle = 2 * M_PI * (tower->time);
				
				//Calculate the tip positions of the hands
				double2 hourTip(sin(hourHandAngle), cos(hourHandAngle));
				double2 minuteTip(sin(minuteHandAngle), cos(minuteHandAngle));
				hourTip *= watchRadius * 0.6;
				minuteTip *= watchRadius;
				hourTip += watchCenter;
				minuteTip += watchCenter;
				
				//Draw the hands
				glColor4f(0, 0, 0, 1);
				glBindTexture(GL_TEXTURE_RECTANGLE_EXT, 0);
				glBegin(GL_LINES);
				glVertex2f(watchCenter.x, watchCenter.y);
				glVertex2f(hourTip.x, hourTip.y);
				glVertex2f(watchCenter.x, watchCenter.y);
				glVertex2f(minuteTip.x, minuteTip.y);
				glEnd();
				
			}
			
		};
	}
}


#endif
