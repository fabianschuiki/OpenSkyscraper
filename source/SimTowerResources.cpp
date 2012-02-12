/**
 *  The bitmaps should be processed and composed according to the following scheme:
 *  - bitmaps that represent different floors of the same item are fused together
 *  - bitmaps for an item are all fused together, so that:
 *    - the item's states (lit, dark, awake, asleep, etc.) are lined up horizontally
 *    - the item's variants (pub, burger, etc.) are lined up vertically
 */
#include <fstream>
#include <sys/stat.h>

#include "Application.h"
#include "logger.h"
#include "SimTowerResources.h"

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

bool SimTowerResources::load(WindowsNEExecutable::ResourceTable & rt)
{
	LOG(DEBUG, "loading SimTower resources");
	
	prepareBitmaps(rt[0x8002]);
	
	loadBitmaps();
	
	return true;
}

/** Prepares the given list of bitmap resources by adding a proper BMP header to each one and storing each in the bitmaps map. */
void SimTowerResources::prepareBitmaps(WindowsNEExecutable::Resources & rs)
{
	WindowsNEExecutable::Resources::iterator r;
	for (r = rs.begin(); r != rs.end(); r++)
	{
		//Create a new bitmap entry.
		Bitmap & bmp = rawBitmaps[r->first];
		bmp.length = r->second.length + 14;
		
		//Store the resource data prefixed with the appropriate BMP header.
		bmp.data = new char [bmp.length];
		for (int i = 0; i < 14; i++)
			bmp.data[i] = 0;
		bmp.data[0]  = 0x42;
		bmp.data[1]  = 0x4D;
		bmp.data[10] = 0x36;
		bmp.data[11] = 0x4;
		memcpy(bmp.data + 14, r->second.data, r->second.length);
	}
}

void SimTowerResources::dump(Path path)
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
	for (Images::iterator i = bitmaps.begin(); i != bitmaps.end(); i++) {
		Path p = bitmapsDir.down(i->first);
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
	for (Bitmaps::iterator b = rawBitmaps.begin(); b != rawBitmaps.end(); b++) {
		snprintf(temp, 16, "%x.bmp", b->first);
		Path p = unhandledBitmapsDir.down(temp);
		
		ofstream f(p.c_str());
		f.write(b->second.data, b->second.length);
		f.close();
	}
}

/** Processes the raw bitmaps and turns them into useable bitmaps. */
void SimTowerResources::loadBitmaps()
{
	//Condos
	sf::Image & condos = bitmaps["condo"];
	condos.Create(5*128, 3*24);
	for (int i = 0; i < 3; i++) {
		sf::Image condo;
		loadCondo(0x8628 + i*5, condo);
		condos.Copy(condo, 0, i*24);
	}
	
	//Offices
	sf::Image & offices = bitmaps["office"];
	offices.Create(144, 7*24);
	unsigned int y = 0;
	for (int i = 0; i < 4; i++) {
		sf::Image office;
		loadOffice(0x85A8 + i, office);
		offices.Copy(office, 0, y);
		y += office.GetHeight();
	}
	
	//Fast Foods
	sf::Image & fastfoods   = bitmaps["fastfood"];
	fastfoods.Create(4*128, 5*24);
	for (int i = 0; i < 5; i++) {
		sf::Image fastfood;
		loadFood(0x86E8 + i*2, fastfood);
		fastfoods.Copy(fastfood, 0, i*24);
	}
	
	//Restaurants
	sf::Image & restaurants = bitmaps["restaurant"];
	restaurants.Create(4*192, 4*24);
	for (int i = 0; i < 4; i++) {
		sf::Image restaurant;
		loadFood(0x856A + i*2, restaurant);
		restaurants.Copy(restaurant, 0, i*24);
	}
	
	//Single hotel rooms
	sf::Image & singleRooms = bitmaps["single"];
	singleRooms.Create(9*32, 2*24);
	for (int i = 0; i < 2; i++) {
		sf::Image hotel;
		loadHotel(0x84A8 + i*2, hotel);
		singleRooms.Copy(hotel, 0, i*24);
	}
	
	//Double hotel rooms
	sf::Image & doubleRooms = bitmaps["double"];
	doubleRooms.Create(9*48, 4*24);
	for (int i = 0; i < 4; i++) {
		sf::Image hotel;
		loadHotel(0x84E8 + i*2, hotel);
		doubleRooms.Copy(hotel, 0, i*24);
	}
	
	//Stairs
	sf::Image stairs[2];
	loadMergedByID('y', stairs[0], 0x8968, 0x89A8, NULL);
	loadMergedByID('y', stairs[1], 0x8969, 0x89A9, NULL);
	loadMerged('x', bitmaps["stairs"], &stairs[0], &stairs[1], NULL);
	
	//Escalator
	loadMergedByID('y', bitmaps["escalator"], 0x8AA8, 0x8AE8, 0);
	loadMergedByID('x', bitmaps["parking_ramp"], 0x8EE8, 0x8EE9, 0x8EEA, NULL);
	loadMergedByID('y', bitmaps["party_hall"], 0x8B28, 0x8B68, NULL);
	
	//Metro
	sf::Image metro[3];
	for (int i = 0; i < 3; i++)
		loadMergedByID('x', metro[i], i*0x40 + 0x8BA9, i*0x40 + 0x8BA8, NULL);
	loadMerged('y', bitmaps["metro"], &metro[0], &metro[1], &metro[2], NULL);
}

void SimTowerResources::loadMerged(char dir, sf::Image & dst, ...)
{
	va_list args;
	
	unsigned int width  = 0;
	unsigned int height = 0;
	sf::Image * img = NULL;
	
	va_start(args, dst);
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
	
	va_start(args, dst);
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

void SimTowerResources::loadMergedByID(char dir, sf::Image & dst, ...)
{
	va_list args;
	
	unsigned int width  = 0;
	unsigned int height = 0;
	std::vector<sf::Image> imgs;
	
	va_start(args, dst);
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

void SimTowerResources::loadCondo(int id, sf::Image & img)
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

void SimTowerResources::loadOffice(int id, sf::Image & img)
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

void SimTowerResources::loadFood(int id, sf::Image & img)
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

void SimTowerResources::loadHotel(int id, sf::Image & img)
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

void SimTowerResources::loadBitmap(int id, sf::Image & img)
{
	Bitmap & bmp = rawBitmaps[id];
	if (!img.LoadFromMemory(bmp.data, bmp.length)) {
		LOG(ERROR, "unable to load bitmap 0x%x from memory", id);
		return;
	}
	rawBitmaps.erase(id);
}
