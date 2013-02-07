/**
 *  The bitmaps should be processed and composed according to the following scheme:
 *  - bitmaps that represent different floors of the same item are fused together
 *  - bitmaps for an item are all fused together, so that:
 *    - the item's states (lit, dark, awake, asleep, etc.) are lined up horizontally
 *    - the item's variants (pub, burger, etc.) are lined up vertically
 */
#include <cassert>
#include <fstream>
#include <sys/stat.h>

//Use libmspack if available to decompress SIMTOWER.EX_
#ifdef MSPACK
extern "C" {
	#include <mspack.h>
}
#endif

#include "Application.h"
#include "Logger.h"
#include "SimTowerLoader.h"

using namespace OT;
using std::ofstream;
using std::string;

//TODO: make this into a FileSystem class or whatever.
bool fileExists(Path p)
{
	struct stat st;
	return stat(p, &st) == 0;
}
bool makeDirectory(Path dir)
{
	if (fileExists(dir))
		return true;
	
	Path parent = dir.up();
	if (!fileExists(parent))
		if (!makeDirectory(parent))
			return false;
	
	if (mkdir(dir, 0777) != 0) {
		LOG(ERROR, "unable to make directory %s", (const char *)dir);
		return false;
	}
	return true;
}

bool SimTowerLoader::load()
{
	LOG(DEBUG, "loading SimTower resources");
	
	//Find the possible paths to the SimTower executable and use the first that works.
	DataManager::Paths paths = app->data.paths("SIMTOWER.EXE");
	bool success = false;
	for (int i = 0; i < paths.size() && !success; i++) {
		success = exe.load(paths[i]);
	}

	//Since no SIMTOWER.EXE was found, try to find a compressed SIMTOWER.EX_.
	DataManager::Paths compressedPaths;
#ifdef MSPACK
	if (!success) {
		//TODO: This is not really portable, do stuff differently.
		Path decompressedPath("/tmp/SIMTOWER.EXE");

		//Iterate through the possible locations and try to decompress each, until one is found.
		compressedPaths = app->data.paths("SIMTOWER.EX_");
		mskwaj_decompressor * d = mspack_create_kwaj_decompressor(NULL);
		if (d) {
			for (int i = 0; i < compressedPaths.size() && !success; i++) {
				const Path &path = compressedPaths[i];
				if (!fileExists(path)) continue;

				LOG(INFO, "Decompressing %s to %s", path.c_str(), decompressedPath.c_str());
				int result = d->decompress(d, path.c_str(), decompressedPath.c_str());
				if (result != MSPACK_ERR_OK) {
					LOG(ERROR, "Unable to decompress file %s.", path.c_str());
				} else {
					success = true;
				}
			}
			mspack_destroy_kwaj_decompressor(d);
		} else {
			LOG(WARNING, "Unable to initialize mskwaj_decompressor. Decompressing SIMROWER.EX_ is therefore not possible.");
		}

		//If the decompression was successful, try to load the resources from there.
		if (success) {
			success = exe.load(decompressedPath);
			if (!success) {
				LOG(WARNING, "Unable to load previously decompressed file %s.", decompressedPath.c_str());
			}
		}
	}
#endif

	if (!success) {
		LOG(WARNING, "Unable to load SimTower executable");
		for (int i = 0; i < paths.size(); i++) {
			LOG(INFO, "  tried %s", paths[i].c_str());
		}
		for (int i = 0; i < compressedPaths.size(); i++) {
			LOG(INFO, "  tried %s (compressed)", compressedPaths[i].c_str());
		}
		return false;
	}
	
	//Prepare the bitmaps. This prefixes bitmap resources with a BMP header.
	preparePalettes();
	prepareBitmaps();
	
	loadBitmaps();
	loadSounds();
	
	return true;
}

void SimTowerLoader::dump(Path path)
{
	LOG(INFO, "dumping to %s", (const char *)path);
	
	//TODO: same issues as for SimTowerResources
	struct stat st;
	char temp[16];
	
	if (stat(path, &st) != 0) {
		LOG(DEBUG, "  creating directory");
		if (mkdir(path, 0777) != 0) {
			LOG(ERROR, "unable to make directory %s", (const char *)path);
			return;
		}
	}
	
	
	Path bitmapsDir = path.down("bitmaps");
	if (stat(bitmapsDir, &st) != 0) {
		if (mkdir(bitmapsDir, 0777) != 0) {
			LOG(ERROR, "unable to make directory %s", (const char *)bitmapsDir);
			return;
		}
	}
	
	//TODO: don't perform the resource dump here, but rather in Application or alike, triggered by a command line argument.
	const BitmapManager::Resources & res = app->bitmaps.getResources();
	for (BitmapManager::Resources::const_iterator i = res.begin(); i != res.end(); i++) {
		Path p = bitmapsDir.down(i->first.str().substr(9));
		Path d = p.up();
		if (!makeDirectory(d))
			continue;
		i->second.SaveToFile(p.str() + ".png");
	}
	
	Path unhandledBitmapsDir = bitmapsDir.down("unhandled");
	if (stat(unhandledBitmapsDir, &st) != 0) {
		if (mkdir(unhandledBitmapsDir, 0777) != 0) {
			LOG(ERROR, "unable to make directory %s", (const char *)unhandledBitmapsDir);
			return;
		}
	} 
	for (Blobs::iterator b = rawBitmaps.begin(); b != rawBitmaps.end(); b++) {
		snprintf(temp, 16, "%x.bmp", b->first);
		Path p = unhandledBitmapsDir.down(temp);
		
		ofstream f(p.c_str());
		f.write(b->second.data, b->second.length);
		f.close();
	}
	
	
	Path soundsDir = path.down("sounds");
	if (stat(soundsDir, &st) != 0) {
		if (mkdir(soundsDir, 0777) != 0) {
			LOG(ERROR, "unable to make directory %s", (const char *)soundsDir);
			return;
		}
	}
	
	//TODO: don't perform the resource dump here, but rather in Application or alike, triggered by a command line argument.
	const SoundManager::Resources & sndres = app->sounds.getResources();
	for (SoundManager::Resources::const_iterator i = sndres.begin(); i != sndres.end(); i++) {
		Path p = soundsDir.down(i->first.str().substr(9));
		Path d = p.up();
		if (!makeDirectory(d))
			continue;
		i->second.SaveToFile(p.str() + ".wav");
	}
	
	Path unhandledSoundsDir = soundsDir.down("unhandled");
	if (stat(unhandledSoundsDir, &st) != 0) {
		if (mkdir(unhandledSoundsDir, 0777) != 0) {
			LOG(ERROR, "unable to make directory %s", (const char *)unhandledSoundsDir);
			return;
		}
	}
	WindowsNEExecutable::Resources & rawSounds = exe.resources[0xFF0A];
	for (WindowsNEExecutable::Resources::iterator b = rawSounds.begin(); b != rawSounds.end(); b++) {
		snprintf(temp, 16, "%x.wav", b->first);
		Path p = unhandledSoundsDir.down(temp);
		
		ofstream f(p.c_str());
		f.write(b->second.data, b->second.length);
		f.close();
	}
}

/** Prepares the given list of bitmap resources by adding a proper BMP header to each one and storing each in the bitmaps map. */
void SimTowerLoader::prepareBitmaps()
{
	LOG(INFO, "Preparing Bitmaps");
	WindowsNEExecutable::Resources::iterator r;
	WindowsNEExecutable::Resources & bmps = exe.resources[0x8002];
	for (r = bmps.begin(); r != bmps.end(); r++)
	{
		//Create a new bitmap entry.
		Blob & bmp = rawBitmaps[r->first];
		bmp.length = r->second.length + 14;
		bmp.data = new char [bmp.length];
		
		//Create the BMP.
		prepareBMPHeader(bmp.data);
		
		//Copy the bitmap.
		memcpy(bmp.data + 14, r->second.data, r->second.length);
	}
	
	WindowsNEExecutable::Resources & pcs = exe.resources[0xFF02];
	for (r = pcs.begin(); r != pcs.end(); r++)
	{
		//Create a new bitmap entry.
		Blob & bmp = rawBitmaps[r->first];
		bmp.length = r->second.length + 0x436;
		bmp.data = new char [bmp.length];
		
		//Create the BMP.
		prepareBMPHeader(bmp.data);
		
		//Calculate the image dimensions.
		const unsigned int cellPixels = 36 * 8;
		unsigned int cellsInResource = r->second.length / cellPixels;
		
		//Create the DIB header.
#ifdef _MSC_VER
		#pragma pack(push, 1)
#endif
		struct {
			uint32_t size;
			uint32_t width;
			uint32_t height;
			uint16_t numPlanes;
			uint16_t bitsPerPixel;
			uint32_t compression;
			uint32_t imageSize;
			uint32_t hDPI;
			uint32_t vDPI;
			uint32_t numColors;
			uint32_t numImportantColors;
		} 
#ifndef _MSC_VER
		__attribute__((__packed__)) 
#endif
		dib = {
			40,
			(cellsInResource * 8), 36,
			1, 8,
			0,
			(cellsInResource * cellPixels),
			0, 0, 256, 256
		};
#ifdef _MSC_VER
		#pragma pack(pop)
#endif
		memcpy(bmp.data + 14, &dib, sizeof(dib));
		
		//Assemble the palette
		Blob & palette = rawPalettes[0x83e8];
		char * dst = (bmp.data + 14 + 40);
		for (int i = 0; i < palette.length; i++) {
			(*dst++) = palette.data[i * 8 + 6];
			(*dst++) = palette.data[i * 8 + 4];
			(*dst++) = palette.data[i * 8 + 2];
			(*dst++) = 0;
		}
		
		//Since the actualy pixel data is only 8 pixels wide, with the individual cells
		//stacked vertically, we re-organize the buffer by flipping the image vertically and
		//lining up the cells horizontally to simplify postprocessing.
		char * src = r->second.data;
		dst = bmp.data + 0x436;
		for (unsigned int i = 0; i < dib.imageSize; i++) {
			unsigned int srcx = (i % 8);
			unsigned int srcy = (i / 8);
			unsigned int dstx = srcx + (srcy / 36) * 8;
			unsigned int dsty = 36 - 1 - (srcy % 36);
			unsigned int dstidx = (dstx + dsty * dib.width);
			dst[dstidx] = src[i];
		}
	}
}

void SimTowerLoader::prepareBMPHeader(char * data)
{
	for (int i = 0; i < 14; i++)
		data[i] = 0;
	data[0]  = 0x42;
	data[1]  = 0x4D;
	data[10] = 0x36;
	data[11] = 0x4;
}

void SimTowerLoader::preparePalettes()
{
	LOG(INFO, "Preparing Palettes");
	WindowsNEExecutable::Resources & rs = exe.resources[0xFF03];
	WindowsNEExecutable::Resources::iterator r;
	for (r = rs.begin(); r != rs.end(); r++)
	{
		Blob & pal = rawPalettes[r->first];
		pal.length = r->second.length;
		pal.data = new char [pal.length];
		memcpy(pal.data, r->second.data, r->second.length);
	}
}

/** Processes the raw bitmaps and turns them into useable bitmaps. */
void SimTowerLoader::loadBitmaps()
{
	LOG(INFO, "Loading Bitmaps");
	//Condos
	sf::Image & condos = app->bitmaps["simtower/condo"];
	condos.Create(5*128, 3*24);
	for (int i = 0; i < 3; i++) {
		sf::Image condo;
		loadCondo(0x8628 + i*5, condo);
		condos.Copy(condo, 0, i*24);
	}
	
	//Offices
	sf::Image & offices = app->bitmaps["simtower/office"];
	offices.Create(144, 7*24);
	unsigned int y = 0;
	for (int i = 0; i < 4; i++) {
		sf::Image office;
		loadOffice(0x85A8 + i, office);
		offices.Copy(office, 0, y);
		y += office.GetHeight();
	}
	
	//Fast Foods
	sf::Image & fastfoods   = app->bitmaps["simtower/fastfood"];
	fastfoods.Create(4*128, 5*24);
	for (int i = 0; i < 5; i++) {
		sf::Image fastfood;
		loadFood(0x86E8 + i*2, fastfood);
		fastfoods.Copy(fastfood, 0, i*24);
	}
	
	//Restaurants
	sf::Image & restaurants = app->bitmaps["simtower/restaurant"];
	restaurants.Create(4*192, 4*24);
	for (int i = 0; i < 5; i++) {
		sf::Image restaurant;
		loadFood(0x8568 + i*2, restaurant);
		restaurants.Copy(restaurant, 0, i*24);
	}
	
	//Single hotel rooms
	sf::Image & singleRooms = app->bitmaps["simtower/single"];
	singleRooms.Create(9*32, 2*24);
	for (int i = 0; i < 2; i++) {
		sf::Image hotel;
		loadHotel(0x84A8 + i*2, hotel);
		singleRooms.Copy(hotel, 0, i*24);
	}
	
	//Double hotel rooms
	sf::Image & doubleRooms = app->bitmaps["simtower/double"];
	doubleRooms.Create(9*48, 4*24);
	for (int i = 0; i < 4; i++) {
		sf::Image hotel;
		loadHotel(0x84E8 + i*2, hotel);
		doubleRooms.Copy(hotel, 0, i*24);
	}
	
	//Suite hotel rooms
	sf::Image suiteRooms[2];
	loadMergedByID(suiteRooms[0], 'x', 0x8528, 0x8529, NULL);
	loadMergedByID(suiteRooms[1], 'x', 0x852A, 0x852B, NULL);
	loadMerged(app->bitmaps["simtower/suite"], 'y', &suiteRooms[0], &suiteRooms[1], NULL);
	
	//Stairs
	sf::Image stairs[2];
	loadMergedByID(stairs[0], 'y', 0x8968, 0x89A8, NULL);
	loadMergedByID(stairs[1], 'y', 0x8969, 0x89A9, NULL);
	loadMerged(app->bitmaps["simtower/stairs"], 'x', &stairs[0], &stairs[1], NULL);
	app->bitmaps["simtower/stairs"].CreateMaskFromColor(sf::Color(0xFF, 0xFF, 0xFF));
	
	//Escalator
	loadMergedByID(app->bitmaps["simtower/escalator"], 'y', 0x8AA8, 0x8AE8, 0);
	app->bitmaps["simtower/escalator"].CreateMaskFromColor(sf::Color(0xFF, 0xFF, 0xFF));
	
	loadMergedByID(app->bitmaps["simtower/parking/ramp"], 'x', 0x8EE8, 0x8EE9, 0x8EEA, NULL);
	loadMergedByID(app->bitmaps["simtower/parking/space"], 'x', 0x86A8, 0x86A9, NULL);
	
	loadMergedByID(app->bitmaps["simtower/partyhall"], 'y', 0x8B28, 0x8B68, NULL);
	
	//Recycling Center
	sf::Image recycling[2];
	loadMergedByID(recycling[0], 'x', 0x88E9, 0x88EA, 0x88EB, 0x88EC, 0x88ED, NULL);
	loadMergedByID(recycling[1], 'x', 0x8929, 0x892A, 0x892B, 0x892C, 0x892D, NULL);
	sf::Image recyclingLoad;
	loadMerged(recyclingLoad, 'y', &recycling[0], &recycling[1], NULL);
	sf::Image recyclingEmpty, recyclingEmptying, recyclingCar;
	loadBitmap(0x88E8, recyclingEmpty);
	loadBitmap(0x892E, recyclingCar);
	recyclingEmptying = recyclingEmpty;
	recyclingEmptying.Copy(recyclingCar, 0, 24);
	loadMerged(app->bitmaps["simtower/recycling"], 'x', &recyclingEmpty, &recyclingLoad, &recyclingEmptying, NULL);
	
	//Metro
	sf::Image metro[3];
	for (int i = 0; i < 3; i++)
		loadMergedByID(metro[i], 'x', i*0x40 + 0x8BA9, i*0x40 + 0x8BA8, NULL);
	loadMerged(app->bitmaps["simtower/metro/station"], 'y', &metro[0], &metro[1], &metro[2], NULL);
	
	//Cinema
	sf::Image cinemaScreens[2];
	for (int i = 0; i < 2; i++)
		loadMergedByID(cinemaScreens[i], 'y', 0x8C68+i, 0x8CA8+i, NULL);
	loadMerged(app->bitmaps["simtower/cinema/screens"], 'x', &cinemaScreens[0], &cinemaScreens[1], NULL);
	
	sf::Image cinemaUpper[3];
	sf::Image cinemaLower;
	loadAnimatedBitmap(0x8868, cinemaUpper);
	loadBitmap(0x88A8, cinemaLower);
	sf::Image & cinema = app->bitmaps["simtower/cinema/hall"];
	cinema.Create(182*5, 60);
	cinema.Copy(cinemaUpper[1], 192, 0);
	cinema.Copy(cinemaUpper[0], 0, 0);
	cinema.Copy(cinemaLower, 192, 24);
	cinema.Copy(cinemaLower, 0, 24);
	
	loadMergedByID(app->bitmaps["simtower/fire/large"], 'x', 0x8F68, 0x8F69, 0x8F6A, 0x8F6B, NULL);
	
	//Load the security offices. They are, among a few other items, special in so far that they are animated by cycling color indices 198, 199 and 200.
	sf::Image security[3];
	loadAnimatedBitmap(0x8768, security);
	loadMerged(app->bitmaps["simtower/security"], 'x', &security[0], &security[1], &security[2], NULL);
	
	//Shops
	sf::Image shops[2];
	loadMergedByID(shops[0], 'x', 0x8673, 0x8674, NULL);
	loadMergedByID(shops[1], 'y', 0x8668, 0x8669, 0x866A, 0x866B, 0x866C, 0x866D, 0x866E, 0x866F, 0x8670, 0x8671, 0x8672, NULL);
	loadMerged(app->bitmaps["simtower/shops"], 'y', &shops[0], &shops[1], NULL);
	
	//Medical Center
	loadMergedByID(app->bitmaps["simtower/medicalcenter"], 'x', 0x8728, 0x8729, 0x872A, NULL);
	
	//People
	sf::Image people[5];
	loadBitmap(0x82BC, people[0]);
	people[0].CreateMaskFromColor(sf::Color(0xE6, 0xE6, 0xE6));
	people[4] = people[3] = people[2] = people[1] = people[0];
	for (int x = 0; x < people[0].GetWidth(); x++) {
		for (int y = 0; y < people[0].GetHeight(); y++) {
			if (people[0].GetPixel(x, y).a > 0) {
				people[1].SetPixel(x, y, sf::Color(0xFF, 0x99, 0x99));
				people[2].SetPixel(x, y, sf::Color(0xFF, 0x00, 0x00));
				people[3].SetPixel(x, y, sf::Color(0x00, 0x00, 0xFF));
				people[4].SetPixel(x, y, sf::Color(0xFF, 0xFF, 0x00));
			}
		}
	}
	loadMerged(app->bitmaps["simtower/people"], 'y', &people[0], &people[1], &people[2], &people[3], &people[4], NULL);
	
	loadElevators();
	
	//Toolbox
	loadMergedByID(app->bitmaps["simtower/ui/toolbox/tools"], 'y', 0x825C, 0x825D, 0x825E, NULL);
	sf::Image speed[2];
	loadMergedByID(speed[0], 'y', 0x8258, 0x8259, NULL);
	loadMergedByID(speed[1], 'y', 0x825A, 0x825B, NULL);
	loadMerged(app->bitmaps["simtower/ui/toolbox/speed"], 'x', &speed[0], &speed[1], NULL);
	
	sf::Image items[3];
	for (int i = 0; i < 3; i++) {
		sf::Image tmp;
		loadBitmap(0x812C+i, tmp);
		items[i].Create(32*26, 32);
		for (int n = 0; n < 4; n++)
			items[i].Copy(tmp, n*256, 0, sf::IntRect(0, n*32, 256, n*32+32));
	}
	loadMerged(app->bitmaps["simtower/ui/toolbox/items"], 'y', &items[0], &items[1], &items[2], NULL);
	
	//Map
	Blob & mapRaw = rawBitmaps[0x8160];
	sf::Image & mapSky = app->bitmaps["simtower/ui/map/sky"];
	mapSky.Create(200*4, 264);
	for (int i = 0; i < 4; i++)
	{
		int rct = 0;
		if (i == 1) rct = 0x83E9;
		if (i == 2) rct = 0x83EA;
		if (i == 3) rct = 0x83EB;
		applyReplacementPalette(rct, mapRaw);
		
		sf::Image tmp;
		if (!tmp.LoadFromMemory(mapRaw.data, mapRaw.length)) {
			LOG(ERROR, "unable to load map sky bitmap from memory");
			return;
		}
		mapSky.Copy(tmp, i*200, 0);
	}
	sf::Image map;
	loadBitmap(0x8160, map);
	sf::Image & mapGround = app->bitmaps["simtower/ui/map/ground"];
	mapGround.Create(200, 24, sf::Color(0, 0, 0, 0));
	mapGround.Copy(map, 0, 0, sf::IntRect(0, 264, 200, 24));
	loadMergedByID(app->bitmaps["simtower/ui/map/buttons"], 'y', 0x8138, 0x8136, 0x8137, NULL);
	loadMergedByID(app->bitmaps["simtower/ui/map/overlays"], 'y', 0x8139, 0x813a, 0x813b, NULL);
	
	//Time Window
	loadBitmap(0x8140, app->bitmaps["simtower/ui/time/bg"]);
	sf::Image stars[2];
	sf::Image starTower;
	loadBitmap(0x8142, stars[0]);
	loadBitmap(0x8143, stars[1]);
	loadBitmap(0x8147, starTower);
	sf::Image & rating = app->bitmaps["simtower/ui/time/rating"];
	rating.Create(108, 22*6, sf::Color(0, 0, 0, 0)); 
	for (int i = 0; i < 5; i++) {
		for (int n = 0; n < 5; n++) {
			int si = (n <= i ? 0 : 1);
			rating.Copy(stars[si], n*21+1, i*22+1);
		}
	}
	rating.Copy(starTower, 0, 22*5);
	rating.CreateMaskFromColor(sf::Color(0x99, 0x99, 0x99));
	rating.CreateMaskFromColor(sf::Color::White);
	
	sf::Image skies[11];
	for (int i = 0; i < 11; i++) {
		loadSky(0x8351+i, skies[i]);
	}
	loadMerged(app->bitmaps["simtower/sky"], 'x', 
		&skies[0], &skies[1], &skies[2],
		&skies[3], &skies[4], &skies[5],
		&skies[6], &skies[7], &skies[8],
		&skies[9], &skies[10], NULL
	);
	
	sf::Image floorTmp;
	loadBitmap(0x83e8, floorTmp);
	sf::Image & floor = app->bitmaps["simtower/floor"];
	floor.Create(8, 36);
	for (int i = 0; i < 8; i += 2) {
		floor.Copy(floorTmp, i, 0, sf::IntRect(16, 0, 2, 36));
	}
	
	for (int n = 0; n < 4; n++) {
		char name[128];
		snprintf(name, 128, "simtower/deco/cloud/%i", n);
		
		sf::Image & cloud = app->bitmaps[name];
		Blob & cloudRaw = rawBitmaps[0x8384+n];
		bool init = false;
		for (int i = 0; i < 4; i++)
		{
			int rct = 0;
			if (i == 1) rct = 0x83E9;
			if (i == 2) rct = 0x83EA;
			if (i == 3) rct = 0x83EB;
			applyReplacementPalette(rct, cloudRaw);
			
			sf::Image tmp;
			if (!tmp.LoadFromMemory(cloudRaw.data, cloudRaw.length)) {
				LOG(ERROR, "unable to load cloud bitmap from memory");
				return;
			}
			if (!init) {
				cloud.Create(tmp.GetWidth(), tmp.GetHeight()*4);
				init = true;
			}
			cloud.Copy(tmp, 0, i*tmp.GetHeight());
		}
		cloud.CreateMaskFromColor(sf::Color::White);
	}
	
	loadLobbies();
	
	sf::Image & elevQueue = app->bitmaps["simtower/elevator/people"];
	sf::Image elevQueueNormal;
	sf::Image elevQueueSecurity;
	loadBitmap(0x8468, elevQueueNormal);
	loadBitmap(0x8469, elevQueueSecurity);
	elevQueue.Create(9*2*16, 3*24, sf::Color::White);
	for (int i = 0; i < 8; i++) {
		for (int n = 0; n < 3; n++) {
			elevQueue.Copy(elevQueueNormal, i*32+16, 0, sf::IntRect(i*80, 12, i*80+16, 36));
			elevQueue.Copy(elevQueueNormal, i*32, 0, sf::IntRect(i*80+64, 12, i*80+80, 36));
			elevQueue.Copy(elevQueueNormal, i*32+16, 24, sf::IntRect(i*80+16, 12, i*80+24, 36));
			elevQueue.Copy(elevQueueNormal, i*32+8, 24, sf::IntRect(i*80+56, 12, i*80+64, 36));
			elevQueue.Copy(elevQueueNormal, i*32+16, 48, sf::IntRect(i*80+24, 12, i*80+40, 36));
			elevQueue.Copy(elevQueueNormal, i*32+0, 48, sf::IntRect(i*80+40, 12, i*80+56, 36));
		}
	}
	elevQueue.CreateMaskFromColor(sf::Color::White);
	for (int x = 0; x < elevQueue.GetWidth(); x++) {
		for (int y = 0; y < elevQueue.GetHeight(); y++) {
			sf::Color c = elevQueue.GetPixel(x, y);
			c.r = 255-c.r;
			c.g = 255-c.g;
			c.b = 255-c.b;
			elevQueue.SetPixel(x, y, c);
		}
	}
	
	const static struct { int id; Path name; sf::Color alpha; } namedBitmaps[] = {
		{0x8E28, "construction/grid",   sf::Color::White},
		{0x8E29, "construction/solid"},
		{0x85EA, "construction/worker", sf::Color::White},
		
		{0x87A8, "housekeeping"},
		
		{0x8F6C, "fire/small"},
		{0x8F6D, "fire/chopper"},
		{0x8FA8, "fire/destroyed"},
		
		{0x8F28, "metro/tracks"},
		
		{0x8388, "deco/santa",      sf::Color::White},
		{0x8389, "deco/skyline",    sf::Color(0x8A, 0xD4, 0xFF)},
		{0x83E9, "deco/entrances",  sf::Color::White},
		{0x83EA, "deco/crane",      sf::Color::White},
		{0x842D, "deco/fireladder", sf::Color::White},
		
		{0xA710, "alerts/terrorist", sf::Color::White},
		{0xA711, "alerts/chopper",   sf::Color(0xCC, 0xCC, 0xCC)},
		{0xA712, "alerts/vip",       sf::Color::White},
		{0xA713, "alerts/treasure",  sf::Color::White},
		{0xA714, "alerts/fire",      sf::Color::White},
		{0xA716, "alerts/starup",    sf::Color(0xCC, 0xCC, 0xCC)},
		{0, ""}
	};
	for (int i = 0; namedBitmaps[i].id != 0; i++) {
		sf::Image & bmp = app->bitmaps[Path("simtower/") + namedBitmaps[i].name];
		loadBitmap(namedBitmaps[i].id, bmp);
		if (namedBitmaps[i].alpha != sf::Color()) {
			bmp.CreateMaskFromColor(namedBitmaps[i].alpha);
		}
	}
}

void SimTowerLoader::loadMerged(sf::Image & dst, char dir, ...)
{
	va_list args;
	
	unsigned int width  = 0;
	unsigned int height = 0;
	sf::Image * img = NULL;
	
	va_start(args, dir);
	do {
		img = va_arg(args, sf::Image *);
		if (img) {
			if (dir == 'x') {
				width  += img->GetWidth();
				height  = std::max<unsigned int>(height, img->GetHeight());
			}
			if (dir == 'y') {
				width   = std::max<unsigned int>(width, img->GetWidth());
				height += img->GetHeight();
			}
		}
	} while (img);
	
	va_start(args, dir);
	dst.Create(width, height);
	unsigned int p = 0;
	do {
		img = va_arg(args, sf::Image *);
		if (img) {
			dst.Copy(*img, (dir == 'x' ? p : 0), (dir == 'y' ? p : 0));
			p += (dir == 'x' ? img->GetWidth() : img->GetHeight());
		}
	} while (img);
	
	va_end(args);
}

void SimTowerLoader::loadMergedByID(sf::Image & dst, char dir, ...)
{
	va_list args;
	
	unsigned int width  = 0;
	unsigned int height = 0;
	std::vector<sf::Image> imgs;
	
	va_start(args, dir);
	for (int i = 0;; i++) {
		int id = va_arg(args, int);
		if (id == 0)
			break;
		imgs.push_back(sf::Image());
		loadBitmap(id, imgs.back());
		if (dir == 'x') {
			width  += imgs[i].GetWidth();
			height  = std::max<unsigned int>(height, imgs[i].GetHeight());
		}
		if (dir == 'y') {
			width   = std::max<unsigned int>(width, imgs[i].GetWidth());
			height += imgs[i].GetHeight();
		}
	}
	va_end(args);
	
	dst.Create(width, height);
	unsigned int p = 0;
	for (int i = 0; i < imgs.size(); i++) {
		dst.Copy(imgs[i], (dir == 'x' ? p : 0), (dir == 'y' ? p : 0));
		p += (dir == 'x' ? imgs[i].GetWidth() : imgs[i].GetHeight());
	}
}

void SimTowerLoader::loadCondo(int id, sf::Image & img)
{
	img.Create(5*128, 24);
	for (int i = 0; i < 5; i++) {
		sf::Image state;
		loadBitmap(id + i, state);
		state.CreateMaskFromColor(sf::Color(0x66, 0x99, 0xCC));
		state.CreateMaskFromColor(sf::Color(0x0C, 0x0C, 0x0C));
		img.Copy(state, i*128, 0);
	}
}

void SimTowerLoader::loadOffice(int id, sf::Image & img)
{
	sf::Image office;
	loadBitmap(id, office);
	office.CreateMaskFromColor(sf::Color(0x8C, 0xD6, 0xFF));
	office.CreateMaskFromColor(sf::Color(0x42, 0xC6, 0xFF));
	unsigned int slices = office.GetWidth() / 144;
	img.Create(144, slices*24);
	for (int i = 0; i < slices; i++) {
		img.Copy(office, 0, i*24, sf::IntRect(i*144, 0, (i+1)*144, 24));
	}
}

void SimTowerLoader::loadFood(int id, sf::Image & img)
{
	for (int i = 0; i < 2; i++) {
		sf::Image food;
		loadBitmap(id + i, food);
		if (i == 0) {
			img.Create(food.GetWidth() * 2, food.GetHeight());
		}
		img.Copy(food, i * food.GetWidth(), 0);
	}
}

void SimTowerLoader::loadHotel(int id, sf::Image & img)
{
	sf::Image first, second;
	loadBitmap(id, first);
	loadBitmap(id+1, second);
	img.Create(first.GetWidth() + second.GetWidth(), 24);
	img.Copy(first, 0, 0);
	img.Copy(second, first.GetWidth(), 0);	
	img.CreateMaskFromColor(sf::Color(0x8C, 0xD6, 0xFF));
	img.CreateMaskFromColor(sf::Color(0x4A, 0xB4, 0xFF));
}

void SimTowerLoader::loadElevators()
{
	sf::Image standard_car, standard_combined_raw[3], service, express_combined[3];
	loadBitmap(0x8428, standard_car);
	loadAnimatedBitmap(0x8429, standard_combined_raw);
	loadAnimatedBitmap(0x842B, express_combined);
	loadBitmap(0x842A, service);
	
	//The standard elevator seems to stem from an earlier phase of SimTower development, since the empty car is in a separate bitmap. We undo that.
	sf::Image standard_combined[3];
	for (int i = 0; i < 3; i++)
		loadMerged(standard_combined[i], 'x', &standard_car, &standard_combined_raw[i], NULL);
	
	//Cut off the engine portion from the standard and express elevator so the cars are all in the same format.
	sf::Image standard, express;
	standard.Create(32*5, 36);
	express. Create(48*5, 36);
	standard.Copy(standard_combined[0], 0, 0);
	express. Copy(express_combined[0], 0, 0);
	
	//Cut out the cars from their surrounding shaft for animation.
	loadElevatorCar(standard, app->bitmaps["simtower/elevator/standard"]);
	loadElevatorCar(service,  app->bitmaps["simtower/elevator/service"]);
	loadElevatorCar(express,  app->bitmaps["simtower/elevator/express"]);
	
	//Render the engines.
	sf::Image & shaft_narrow = app->bitmaps["simtower/elevator/narrow"]; 
	sf::Image & shaft_wide   = app->bitmaps["simtower/elevator/wide"];
	shaft_narrow.Create(32*7, 36);
	shaft_wide.  Create(48*7, 36);
	for (int i = 0; i < 3; i++) {
		shaft_narrow.Copy(standard_combined[i], i*2*32+32, 0, sf::IntRect(5*32, 0, 7*32, 36));
		shaft_wide  .Copy(express_combined[i],  i*2*48+48, 0, sf::IntRect(5*48, 0, 7*48, 36));
	}
	
	//Render the shafts. The wide shaft requires some copy-pasting of the narrow shaft.
	sf::Image shaft;
	sf::Image extension;
	loadBitmap(0x87E8, shaft);
	loadBitmap(0x842c, extension);
	shaft_narrow.Copy(shaft, 0, 0, sf::IntRect(0, 0, 32, 36));
	shaft_wide.Copy(shaft, 8,  0, sf::IntRect(0,  0, 32, 36));
	shaft_wide.Copy(extension, 0, 0, sf::IntRect(0, 0, 8, 36));
	shaft_wide.Copy(extension, 40, 0, sf::IntRect(8, 0, 16, 36));
	
	//Render the shaft digits.
	sf::Image rawDigits[2];
	loadBitmap(0x87e9, rawDigits[0]);
	loadBitmap(0x87ec, rawDigits[1]);
	sf::Image & digits = app->bitmaps["simtower/elevator/digits"];
	digits.Create(11*12, 2*17);
	for (int i = 0; i < 10; i++) {
		for (int n = 0; n < 2; n++) {
			digits.Copy(rawDigits[n], i*11, n*17, sf::IntRect(1+16*i, 16, 1+16*i+11, 33));
		}
	}
	
	sf::Image rawFirstDigits[2];
	loadBitmap(0x87ea, rawFirstDigits[0]);
	loadBitmap(0x87ed, rawFirstDigits[1]);
	for (int n = 0; n < 2; n++) {
		digits.Copy(rawFirstDigits[n], 110, n*17, sf::IntRect(36, 16, 47, 33));
	}
	
	digits.CreateMaskFromColor(sf::Color(25, 25, 25));
}

void SimTowerLoader::loadElevatorCar(const sf::Image & img, sf::Image & dst)
{
	unsigned int w = img.GetWidth() / 5;
	unsigned int h = img.GetHeight();
	unsigned int carw = w - 2 - 2;
	unsigned int carh = h - 1 - 5;
	dst.Create(carw*5, carh);
	for (int i = 0; i < 5; i++) {
		dst.Copy(img, i*carw, 0, sf::IntRect(i*w + 2, 5, (i+1)*w - 2, h - 1));
	}
}

void SimTowerLoader::applyReplacementPalette(int palette, Blob & raw)
{
	if (palette <= 0) return;
	Blob * rct = &rawPalettes[palette];
	assert(rct && "rawPalette does not exist");
	
	for (int n = 0; n < 256; n++) {
		//For some reason index 184 exists twice in the color palette, so we skip that color.
		//No idea why this is there though.
		unsigned int ridx = n;
		if (ridx >= 184) ridx++;
		ridx = ridx % 256;
		
		for (int t = 0; t < 4; t++) {
			raw.data[0x36 + n*4 + 3 - t] = rct->data[ridx*8 + t*2];
		}
	}
}

void SimTowerLoader::loadSky(int id, sf::Image & img)
{
	Blob & raw = rawBitmaps[id];
	
	//Locate the sky colors responsible for raindrops and sky background.
	char * psky    = (raw.data + 0x36 + 4*188);
	char * pdark   = (raw.data + 0x36 + 4*207);
	char * pbright = (raw.data + 0x36 + 4*213);
	
	//Make a backup copy of the raindrops as they will be overwritten.
	char cdark[24];   memcpy(cdark,   pdark,   24);
	char cbright[24]; memcpy(cbright, pbright, 24);
	
	//Assemble the image.
	img.Create(32*6, 360);
	for (int i = 0; i < 6; i++)
	{
		//Choose a replacement palette which yields twilight, night and overcast colorings.
		/*Blob * rct = NULL;
		if (i == 1) rct = &rawPalettes[0x83E9];
		if (i == 2) rct = &rawPalettes[0x83EA];
		if (i == 3) rct = &rawPalettes[0x83EB];
		
		//Apply replacement palette if one was picked. 
		if (rct) {
			for (int n = 0; n < 256; n++) {
				//For some reason index 184 exists twice in the color palette, so we skip that color.
				//No idea why this is there though.
				unsigned int ridx = n;
				if (ridx >= 184) ridx++;
				ridx = ridx % 256;
				
				for (int t = 0; t < 4; t++) {
					raw.data[0x36 + n*4 + 3 - t] = rct->data[ridx*8 + t*2];
				}
			}
		}*/
		int rct = 0;
		if (i == 1) rct = 0x83E9;
		if (i == 2) rct = 0x83EA;
		if (i == 3) rct = 0x83EB;
		applyReplacementPalette(rct, raw);
		
		//Make the raindrops have the same color as the sky for the first 4 bitmaps, which are day, twilight, night and overcast without rain states.
		if (i < 4) {
			memcpy(pdark,   psky, 24);
			memcpy(pbright, psky, 24);
		}
		
		//There are two sets of raindrops, which are highlighted in turns. This gives the ugly SimTower rain animation where there are two raindrop sprites which are drawn in turns.
		if (i == 4) {
			memcpy(pdark,   cbright, 24);
		}
		if (i == 5) {
			memcpy(pdark,   psky,    24);
			memcpy(pbright, cbright, 24);
		}
		
		//Copy the modified sky bitmap onto the final bitmap.
		sf::Image tmp;
		if (!tmp.LoadFromMemory(raw.data, raw.length)) {
			LOG(ERROR, "unable to load bitmap 0x%x from memory", id);
			return;
		}
		img.Copy(tmp, i*32, 0);
	}
	rawBitmaps.erase(id);
}

void SimTowerLoader::loadLobbies()
{
	sf::Image & normal = app->bitmaps["simtower/lobby/normal"];
	sf::Image & sky    = app->bitmaps["simtower/lobby/sky"];
	sf::Image & high   = app->bitmaps["simtower/lobby/high"];
	normal.Create(312, 3*36);
	sky   .Create(312, 3*36);
	high  .Create(312, 3*108);
	
	sf::Image * segments[] = {&normal, &sky, &high};
	
	for (int i = 0; i < 3; i++) {
		sf::Image raw;
		loadBitmap(0x89e8 + i, raw);
		for (int n = 0; n < 3; n++) {
			int dsty = (n < 2 ? i*36 : i*108+72);
			segments[n]->Copy(raw, 7*8, dsty, sf::IntRect(n*328, 0, (n+1)*328-9*8, 36));
			segments[n]->Copy(raw, 0,   dsty, sf::IntRect((n+1)*328-7*8, 0, (n+1)*328, 36));
		}
	}
	sky.CreateMaskFromColor(sf::Color(0x8C, 0xD6, 0xFF));
	
	for (int i = 0; i < 3; i++) {
		sf::Image middle, top;
		loadBitmap(0x8a28 + i, middle);
		loadBitmap(0x8a68 + i, top);
		sf::Image * vsegments[] = {&top, &middle};
		for (int n = 0; n < 2; n++) {
			int xoff = (n == 1 ? 328 : 0);
			high.Copy(*vsegments[n], 7*8, i*108 + n*36, sf::IntRect(xoff, 0, xoff + 328-9*8, 36));
			high.Copy(*vsegments[n], 0, i*108 + n*36, sf::IntRect(xoff + 328-7*8, 0, xoff + 328, 36));
		}
	}
}

void SimTowerLoader::loadBitmap(int id, sf::Image & img)
{
	Blob & bmp = rawBitmaps[id];
	if (!img.LoadFromMemory(bmp.data, bmp.length)) {
		LOG(ERROR, "unable to load bitmap 0x%x from memory", id);
		return;
	}
	rawBitmaps.erase(id);
}

/** Certain bitmaps in SimTower are animated by swapping colors in the color table. The following colors are rotated to achieve animation:
 *  - 197, 198
 *  - 199, 200
 *  - 201, 202, 203 */
void SimTowerLoader::loadAnimatedBitmap(int id, sf::Image img[3])
{
	Blob & bmp = rawBitmaps[id];
	for (int i = 0; i < 3; i++) {
		for (int n = 0; n < 4; n++) {
			size_t o = 0x36 + 4 * 197 + n;
			char temp;
			
			temp = bmp.data[o + 4];
			bmp.data[o + 4] = bmp.data[o];
			bmp.data[o] = temp;
			
			temp = bmp.data[o + 12];
			bmp.data[o + 12] = bmp.data[o + 8];
			bmp.data[o + 8] = temp;
			
			temp = bmp.data[o + 24];
			bmp.data[o + 24] = bmp.data[o + 20];
			bmp.data[o + 20] = bmp.data[o + 16];
			bmp.data[o + 16] = temp;
		}
		if (!img[i].LoadFromMemory(bmp.data, bmp.length)) {
			LOG(ERROR, "unable to load bitmap 0x%x from memory", id);
			return;
		}
	}
	rawBitmaps.erase(id);
}


void SimTowerLoader::loadSounds()
{
	LOG(INFO, "Loading Sounds");
	const static struct { int id; Path name; } namedSounds[] = {
		{0x9b58, "construction/normal"},		//sound for placing facilities and transport
		{0x9b59, "construction/flexible"},		//sound for flexible stuff like lobbies and floors
		{0x9b5a, "construction/impossible"},	//annoying "click" :)
		{0x9b5b, "bulldozer"},
		
		{0x8568, "restaurant"},
		{0x85a8, "office"},
		{0x8ba8, "metro"},
		{0x8b28, "partyhall"},
		{0x8569, "fastfood/0"},
		{0x9f40, "fastfood/1"},
		{0x8668, "fastfood/2"},

		{0x86a8, "car/departing"},
		{0x86a9, "car/arriving"},

		{0x8628, "doorbell"},
		{0x88e8, "hover"},
		{0x8629, "toilet"},
		
		{0x9388, "birds/morning"},
		{0x9389, "cock"},
		{0x938a, "birds/evening"}, //it is probably a nightingale, but my ornithological knowledge is quite sparse.
		{0x938b, "rain"},
		{0x938c, "bells"},
		{0x938d, "thunder"},
		{0xa71b, "crickets"},
		{0xa71c, "birds/day"},
		
		{0xa710, "rating/increased"},
		{0xa718, "rating/tower"},
		{0xa711, "applause"},
		{0xa712, "santa"},
		{0xa71d, "cash"},
		{0xa715, "wind"},

		{0xa329, "cinema/movie0"},
		{0xa32a, "cinema/movie1"},
		{0xa32b, "cinema/movie2"},
		{0xa32c, "cinema/movie3"},
		{0xa32d, "cinema/movie4"},
		{0xa32e, "cinema/movie5"},
		{0xa32f, "cinema/movie6"},
		{0xa330, "cinema/movie7"},
		{0xa331, "cinema/movie8"},
		{0xa332, "cinema/movie9"},
		{0xa333, "cinema/movie10"},
		{0xa334, "cinema/movie11"},
		{0xa335, "cinema/movie12"},
		{0xa336, "cinema/movie13"},
		{0xa337, "cinema/movie14"},

		{0xce20, "splashscreen"},
		
		{0x9771, "elevator/arriving"},
		{0x9772, "elevator/departing"},
		{0, ""}
	};
	for (int i = 0; namedSounds[i].id != 0; i++) {
		sf::SoundBuffer & snd= app->sounds[Path("simtower/") + namedSounds[i].name];
		loadSound(namedSounds[i].id, snd);
	}
}

void SimTowerLoader::loadSound(int id, sf::SoundBuffer & snd)
{
	WindowsNEExecutable::Resource & data = exe.resources[0xFF0A][id];
	if (!snd.LoadFromMemory(data.data, data.length)) {
		LOG(ERROR, "unable to load sound 0x%x from memory", id);
		return;
	}
	exe.resources[0xFF0A].erase(id);
}
