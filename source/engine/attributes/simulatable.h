#ifndef OSS_ENGINE_ATTRIBUTES_SIMULATABLE_H
#define OSS_ENGINE_ATTRIBUTES_SIMULATABLE_H


namespace OSS {
	class Simulatable {
	public:
		virtual void advance(double dt) {}
	};
}


#endif
