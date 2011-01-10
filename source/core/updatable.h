#ifndef OSS_CORE_UPDATABLE_H
#define OSS_CORE_UPDATABLE_H


namespace OSS {
	namespace Core {
		class Updatable {
			
			/**
			 * Updatable objects have an at least two internal states which may get out of sync.
			 * The updating mechanism is a means of scheduling and performing a state sync. For
			 * example an item might change 10 states in a single simulation iteration, but its
			 * textures only need to be updated at the very end.
			 */
		public:
			virtual void init() {}
			virtual void update() {}
			
			
			/**
			 * Conditional state updates are convenience wrapper objects that wrap around an update
			 * function of a class and provide a means of marking a certain update procedure as
			 * being necessary.
			 */
		public:
			template <class T> class Conditional {
			private:
				T * const object;
				typedef void (T::*functionType)();
				functionType const function;
				
				bool needed;
				
			public:
				Conditional(T * object, functionType function) :
				object(object), function(function), needed(true) {}
				
				bool isNeeded() { return needed; }
				void setNeeded() { needed = true; }
				
				void execute() { needed = false; (object->*function)(); }
				void operator ()() { if (needed) execute(); }
			};
			
			
			/**
			 * Conditional basic updates.
			 */
		public:
			Conditional<Updatable> updateIfNeeded;
			
			Updatable() : updateIfNeeded(this, &Updatable::update) {}
		};
	}
}


#endif
