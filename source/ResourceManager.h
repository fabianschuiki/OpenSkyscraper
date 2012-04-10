#pragma once
#include <map>
#include <string>
#include "Path.h"

namespace OT
{
	class Application;
	
	template <typename T>
	class ResourceManager
	{
	public:
		Application * const app;
		ResourceManager(Application * app) : app(app) {}
		
		T & get(Path name) {
			if (!resources.count(name)) {
				load(name, resources[name]);
			}
			return resources[name];
		}
		
		T & operator [] (Path name) {
			return get(name);
		}
		
		typedef std::map<Path, T> Resources;
		Resources & getResources() { return resources; }
		
	protected:
		virtual bool load(Path name, T & dst) { return true; }
		
	private:
		Resources resources;
	};
}
