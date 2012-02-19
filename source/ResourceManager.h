#pragma once
#include <map>
#include <string>

namespace OT {
	template <typename T>
	class ResourceManager
	{
	public:
		T & get(Path name) {
			if (!resources.count(name)) {
				load(name, resources[name]);
			}
			return resources[name];
		}
		
		T & operator [] (Path name) {
			return get(name);
		}
		
	protected:
		virtual bool load(Path name, T & dst) { return true; }
		
	private:
		std::map<Path, T> resources;
	};
}
