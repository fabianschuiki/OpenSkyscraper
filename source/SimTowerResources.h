#pragma once

#include <map>
#include <SFML/Window.hpp>
#include <string>

#include "Path.h"
#include "WindowsNEExecutable.h"

namespace OT
{
	class Application;
	class SimTowerResources
	{
	public:
		Application * const app;
		SimTowerResources(Application * app) : app(app) {}
		
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
		
		bool load();
		void dump(Path path);
		
	private:
		WindowsNEExecutable exe;
		
		void prepareBitmaps();
		void preparePalettes();
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
