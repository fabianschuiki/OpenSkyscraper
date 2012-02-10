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
	loadBitmap(0x85E9, "condo/inhabitants");
	loadCondo(0x8628, "0");
	loadCondo(0x862D, "1");
	loadCondo(0x8632, "2");
}

void SimTowerResources::loadCondo(int id, string name)
{
	Path base("condo");
	base.append(name);
	loadBitmap(id + 0, base.down("day/inhabited"));
	loadBitmap(id + 2, base.down("night/awake"));
	loadBitmap(id + 3, base.down("night/asleep"));
	loadBitmap(id + 4, base.down("day/forsale"));
	loadBitmap(id + 1, base.down("night/forsale"));
}

void SimTowerResources::loadBitmap(int id, string name)
{
	Bitmap & bmp    = rawBitmaps[id];
	sf::Image & img = bitmaps[name];
	if (!img.LoadFromMemory(bmp.data, bmp.length)) {
		LOG(ERROR, "unable to load bitmap 0x%x (aka %s) from memory", id, name.c_str());
		return;
	}
	rawBitmaps.erase(id);
}
