/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "Path.h"
#include "WindowsNEExecutable.h"
#include <map>
#include <SFML/Window.hpp>
#include <string>

namespace OT
{
	class Application;

	/**
	 * @brief Extracts the resources from a SIMTOWER.EXE.
	 *
	 * The SimTowerLoader class extracts all the required resources from an
	 * original version of SimTower (SIMTOWER.EXE). This includes bitmaps,
	 * palettes and sounds. The bitmaps are rearranged and have various
	 * palettes applied before they are transformed into resources.
	 */
	class SimTowerLoader
	{
	public:
		/// Pointer to the Application instance.
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
		
		void loadMerged(sf::Image & dst, char dir, ...);
		void loadMergedByID(sf::Image & dst, char dir, ...);
		
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
