#pragma once

#include <map>
#include <SFML/Window.hpp>
#include <string>

#include "Path.h"
#include "WindowsNEExecutable.h"

namespace OT
{
	class SimTowerResources
	{
	public:
		struct Bitmap {
			char * data;
			int length;
			Bitmap() { data = NULL; }
			~Bitmap() { if (data) delete data; }
		};
		typedef std::map<int, Bitmap> Bitmaps;
		
		typedef std::map<Path, sf::Image> Images;
		
		Bitmaps rawBitmaps;
		Images bitmaps;
		
		bool load(WindowsNEExecutable::ResourceTable & rt);
		void dump(Path path);
		
	private:
		void prepareBitmaps(WindowsNEExecutable::Resources & rs);
		void loadBitmaps();
		void loadCondo(int id, std::string name);
		void loadBitmap(int id, std::string name);
	};
}
