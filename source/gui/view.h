#ifndef OSS_GUI_VIEW_H
#define OSS_GUI_VIEW_H

#include "external.h"


namespace OSS {
	namespace GUI {
		class View : public Engine::Responder {
			
			/**
			 * Types
			 */
		public:
			//List
			typedef std::list< Pointer<View> > List;
			
			//Positioning
			typedef enum {
				Above	= 1,
				Below	= -1
			} OrderingMode;
			
			//Autoresizing Mask
			typedef enum {
				NotSizable	= 0,
				MinXFixed	= (1 << 0),
				MaxXFixed	= (1 << 1),
				MinYFixed	= (1 << 2),
				MaxYFixed	= (1 << 3)
			} AutoresizingMask;
			
			
			/**
			 * Hierarchy
			 *
			 * Modifying the view hierarchy.
			 */
		private:
			Pointer<View> superview;
			List subviews;
			
		public:
			//Superview
			View * getSuperview();
			void setSuperview(View * superview);
			
			virtual void willMoveToSuperview(View * superview) {}
			virtual void didMoveToSuperview() {}
			
			void removeFromSuperview();
			
			bool isDescendantOf(View * view);
			View * getRootView();
			
			//Subviews
			const List & getSubviews();
			void setSubviews(const List& subviews);
			
			void addSubview(View * subview);
			void addSubview(View * subview, OrderingMode positioned, View * relativeTo);
			
			virtual void willAddSubview(View * subview) {}
			virtual void didAddSubview(View * subview) {}
			
			void removeSubview(View * subview);
			
			virtual void willRemoveSubview(View * subview) {}
			virtual void didRemoveSubview(View * subview) {}
			
			
			/**
			 * Frame
			 *
			 * Modifying the frame rectangle.
			 */
		private:
			rectd frame;
			
		public:
			//As a whole
			const rectd & getFrame();
			void setFrame(const rectd & frame);
			
			virtual void willChangeFrame(const rectd & newFrame) {}
			virtual void didChangeFrame(const rectd & oldFrame) {}
			
			//Origin
			const double2 & getFrameOrigin();
			void setFrameOrigin(const double2 & origin);
			
			virtual void willChangeFrameOrigin(const double2 & newOrigin) {}
			virtual void didChangeFrameOrigin(const double2 & oldOrigin) {}
			
			//Size
			const double2 & getFrameSize();
			void setFrameSize(const double2 & size);
			
			virtual void willChangeFrameSize(const double2 & newSize) {}
			virtual void didChangeFrameSize(const double2 & oldSize);
			
			
			/**
			 * Coordinate Conversion
			 */
		public:
			double2 convertFrom(double2 remote, View * from);
			double2 convertTo(double2 local, View * to);
			
			rectd convertFrom(rectd remote, View * from);
			rectd convertTo(rectd local, View * to);
			
			
			/**
			 * Layout
			 */
		private:
			unsigned int autoresizingMask;
			
		public:
			unsigned int getAutoresizingMask();
			void setAutoresizingMask(unsigned int autoresizingMask);
			
			virtual void layoutSubviewsWithOldSize(double2 oldSize);
			virtual void layoutWithOldSuperviewSize(double2 oldSize);
			
			
			/**
			 * Drawing
			 *
			 * A view is supposed to draw itself and all its subviews in the draw() function. It is
			 * at the subclasser's discretion whether to draw the view or the subviews first, al-
			 * though the former is recommended.
			 *
			 * Before calling draw() on its subviews, the view should translate the current OpenGL
			 * modelview matrix to the origin of the subview.
			 */
		public:
			virtual void draw(rectd dirtyRect);
			
			
			/**
			 * Event Sending
			 */
		public:
			virtual bool sendEventToNextResponders(Base::Event * event);
		};
	}
}


#endif
