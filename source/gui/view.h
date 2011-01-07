#ifndef OSS_GUI_VIEW_H
#define OSS_GUI_VIEW_H


namespace OSS {
	namespace GUI {
		class View {
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
				NotSizable		= 0,
				MinXFlexible	= (1 << 0),
				MaxXFlexible	= (1 << 1),
				MinYFlexible	= (1 << 2),
				MaxYFlexible	= (1 << 3),
				WidthFlexible	= (1 << 4),
				HeightFlexible	= (1 << 5)
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
			
			void replaceSubview(View * subview, View * with);
			
			
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
			void setFrameOrigin(const double2 & frame);
			
			virtual void willChangeFrameOrigin(const double2 & newOrigin) {}
			virtual void didChangeFrameOrigin(const double2 & oldOrigin) {}
			
			//Size
			const double2 & getFrameSize();
			void setFrameSize(const double2 & frame);
			
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
			 * Rendering
			 *
			 * The rendering mechanism is used to tell a view hierarchy to draw itself. It does this
			 * by asking the view followed by its subviews to draw itself.
			 */
		public:
			void render(rectd rect);
			
			virtual void willRender(rectd dirtyRect) {}
			virtual void didRender(rectd dirtyRect) {}
			
			
			/**
			 * Drawing
			 */
		public:
			virtual void draw(rectd rect);
			
			virtual void willDraw(rectd rect) {}
			virtual void didDraw(rectd rect) {}
		};
	}
}


#endif
