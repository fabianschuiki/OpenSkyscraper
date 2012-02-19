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
		struct Blob {
			char * data;
			int length;
			Blob() { data = NULL; length = 0; }
			~Blob() { if (data) delete data; data = NULL; }
		};
		typedef std::map<int, Blob> Bitmaps;
		typedef std::map<int, Blob> Blobs;
		typedef std::map<Path, sf::Image> Images;
		
		Bitmaps rawBitmaps;
		Blobs rawPalettes;
		Images bitmaps;
		
		bool load(WindowsNEExecutable::ResourceTable & rt);
		void dump(Path path);
		
	private:
		void prepareBitmaps (WindowsNEExecutable::Resources & rs);
		void preparePalettes(WindowsNEExecutable::Resources & rs);
		void loadBitmaps();
		
		void loadMerged(char dir, sf::Image & dst, ...);
		void loadMergedByID(char dir, sf::Image & dst, ...);
		
		void loadCondo(int id, sf::Image & img);
		void loadOffice(int id, sf::Image & img);
		void loadFood(int id, sf::Image & img);
		void loadHotel(int id, sf::Image & img);
		void loadElevators();
		void loadElevatorCar(const sf::Image & img, sf::Image & dst);
		void loadSky(int id, sf::Image & img);
		
		void loadBitmap(int id, sf::Image & img);
		void loadAnimatedBitmap(int id, sf::Image img[3]);
	};
}
