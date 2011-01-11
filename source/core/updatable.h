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
			class AbstractConditional {
			protected:
				AbstractConditional * parent;
				bool needed;
				
			public:
				AbstractConditional() : parent(NULL), needed(true) {}
				AbstractConditional(AbstractConditional * parent) : parent(parent), needed(true) {}
				
				bool isNeeded() { return needed; }
				void setNeeded() { needed = true; if (parent) parent->setNeeded(); }
				
				virtual void execute() = 0;				
				void operator ()() { if (needed) execute(); }
			};
			
			template <class T> class Conditional : public AbstractConditional {
			private:
				T * const object;
				typedef void (T::*functionType)();
				functionType const function;
				
			public:
				Conditional(T * object, functionType function) :
				AbstractConditional(), object(object), function(function) {}
				Conditional(T * object, functionType function, AbstractConditional * parent) :
				AbstractConditional(parent), object(object), function(function) {}
				
				void execute() { needed = false; (object->*function)(); }
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
