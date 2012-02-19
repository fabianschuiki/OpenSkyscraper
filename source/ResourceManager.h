#pragma once
#include <map>
#include <string>

namespace OT {
	template <typename T>
	class ResourceManager
	{
	public:
		T & get(std::string name) {
			if (!resources.count(name)) {
				load(name, resources[name]);
			}
			return &resources[name];
		}
		
		T & operator [] (std::string name) {
			return get(name);
		}
		
	protected:
		virtual bool load(std::string name, T & dst) { return true; }
		
	private:
		std::map<std::string, T> resources;
	};
}
