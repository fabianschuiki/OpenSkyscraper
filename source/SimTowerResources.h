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
		void loadMerged(char dir, sf::Image & dst, ...);
		void loadMergedByID(char dir, sf::Image & dst, ...);
		void loadCondo(int id, sf::Image & img);
		void loadOffice(int id, sf::Image & img);
		void loadFood(int id, sf::Image & img);
		void loadHotel(int id, sf::Image & img);
		void loadElevators();
		void loadElevatorCar(const sf::Image & img, sf::Image & dst);
		
		void loadBitmap(int id, sf::Image & img);
		void loadAnimatedBitmap(int id, sf::Image img[3]);
	};
}
