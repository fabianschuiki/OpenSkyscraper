/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "Path.h"
#include <map>
#include <string>

namespace OT
{
	class Application;
	
	/**
	 * @brief Manages a set of resources of one specific type.
	 *
	 * A subclass of ResourceManager loads and maintains resources of type T.
	 * Whenever a resource is requested through the get() function that is not
	 * yet loaded, the virtual load() function is called to give the subclass
	 * the chance to properly load the resource.
	 */
	template <typename T>
	class ResourceManager
	{
	public:
		/// A pointer to the Application instance.
		Application * const app;
		ResourceManager(Application * app) : app(app) {}
		
		/**
		 * @brief Returns the resource given by name.
		 *
		 * If the resource is not yet loaded, calls the load() function for the
		 * resource to load it prior to returning it.
		 *
		 * @param name  Path or name of the resource to load.
		 * @return Returns a reference to a resource of type T.
		 */
		T & get(Path name) {
			if (!resources.count(name)) {
				load(name, resources[name]);
			}
			return resources[name];
		}
		
		/// Convenience wrapper around get().
		T & operator [] (Path name) {
			return get(name);
		}
		
		/// The type of the resource map maintained by the ResourceManager.
		typedef std::map<Path, T> Resources;

		/// Returns a reference to the resource map.
		Resources & getResources() { return resources; }
		
	protected:
		/**
		 * @brief Loads the resource name into dst.
		 *
		 * Called whenever a resource needs to be loaded. This function should
		 * be overridden by subclasses to properly load the resource.
		 *
		 * @param name  Path or name of the resource to load.
		 * @param dst   Reference to the destination resource object to be
		 *              filled in.
		 * @return Returns true if the resource was loaded successfully, false
		 *         otherwise.
		 */
		virtual bool load(Path name, T & dst) { return true; }
		
	private:
		Resources resources;
	};
}
