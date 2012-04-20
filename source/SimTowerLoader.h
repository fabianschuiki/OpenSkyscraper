#pragma once

#include <map>
#include <SFML/Window.hpp>
#include <string>

#include "Path.h"
#include "WindowsNEExecutable.h"

namespace OT
{
	class Application;
	class SimTowerLoader
	{
	public:
		Application * const app;
		SimTowerLoader(Application * app) : app(app) {}
		
		struct Blob {
			char * data;
			int length;
			Blob() { data = NULL; length = 0; }
			~Blob() { if (data) delete data; data = NULL; }
		};
		typedef std::map<int, Blob> Blobs;
		typedef std::map<Path, sf::Image> Images;
		
		Blobs rawBitmaps;
		Blobs rawPalettes;
		Blobs rawSounds;
		
		bool load();
		void dump(Path path);
		
	private:
		WindowsNEExecutable exe;
		
		void prepareBitmaps();
		void prepareBMPHeader(char * data);
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
		void applyReplacementPalette(int palette, Blob & raw);
		void loadSky(int id, sf::Image & img);
		void loadLobbies();
		
		void loadBitmap(int id, sf::Image & img);
		void loadAnimatedBitmap(int id, sf::Image img[3]);
		
		void loadSounds();
		void loadSound(int id, sf::SoundBuffer & snd);
	};
}
