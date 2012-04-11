#pragma once
#include <Rocket/Core/Decorator.h>
#include <Rocket/Core/DecoratorInstancer.h>

namespace OT {
	
	class TimeWindowWatch : public Rocket::Core::Decorator
	{
	public:
		virtual Rocket::Core::DecoratorDataHandle GenerateElementData(Rocket::Core::Element* element) { return 0; };
		virtual void ReleaseElementData(Rocket::Core::DecoratorDataHandle element_data) {};
		virtual void RenderElement(Rocket::Core::Element* element, Rocket::Core::DecoratorDataHandle element_data);
	};
	
	class TimeWindowWatchInstancer : public Rocket::Core::DecoratorInstancer
	{
	public:
		virtual Rocket::Core::Decorator* InstanceDecorator(const Rocket::Core::String& name, const Rocket::Core::PropertyDictionary& properties) {
			return new TimeWindowWatch();
		}
		virtual void ReleaseDecorator(Rocket::Core::Decorator* decorator) { delete decorator; }
		virtual void Release() { delete this; }
	};
}
