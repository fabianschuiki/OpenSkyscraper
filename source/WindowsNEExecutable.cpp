#include <fstream>
#include <sys/stat.h>

#include "Application.h"
#include "Logger.h"
#include "WindowsNEExecutable.h"

using namespace OT;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::ios_base;

bool WindowsNEExecutable::load(Path path)
{
	LOG(DEBUG, "from %s", (const char *)path);
	
	//Open the file for reading.
	ifstream f(path.c_str(), ios_base::in | ios_base::binary);
	if (!f.is_open())
		return false;
	
	//Read the offset at which the segmented EXE header is located.
	uint32_t off_sh;
	f.seekg(0x3C, ios::beg);
	f.read((char *)&off_sh, sizeof(off_sh));
	//TODO: find some little-endian swapping and do that after each f.read operation.
	//off_sh = swapLE32(off_sh);
	LOG(DEBUG, "header:\n  off_sh = 0x%x", off_sh);
	if (f.eof())
		goto unexpected_eof;
	
	//Read the offset at which the resource table is located, as well as the logical sector alignment.
	uint16_t off_rt;
	uint16_t lsa;
	f.seekg(off_sh + 0x24, ios::beg);
	f.read((char *)&off_rt, sizeof(off_rt));
	f.seekg(off_sh + 0x32, ios::beg);
	f.read((char *)&lsa, sizeof(lsa));
	LOG(DEBUG, "segmented header:\n  off_rt = 0x%x\n  lsa = %i", off_rt, lsa);
	if (f.eof())
		goto unexpected_eof;
	
	//Read the entire resource table.
	LOG(DEBUG, "resource table:");
	f.seekg(off_sh + off_rt + 2, ios::beg);
	while (!f.eof())
	{
		//Read the type and number of resources this resource list contains.
		uint16_t type, count;
		f.read((char *)&type,  sizeof(type));
		f.read((char *)&count, sizeof(count));
		if (f.eof())
			goto unexpected_eof;
		
		//A resource type of 0 marks the end of the list.
		if (type == 0)
			break;
		
		LOG(DEBUG, "  type 0x%x: %i resources", type, count);
		
		//Read the resources in this list.
		f.seekg(4, ios::cur);
		for (int i = 0; i < count; i++)
		{
			//Read where the resource is located in the file and how long it is.
			uint16_t offset_aligned, length_aligned;
			f.read((char *)&offset_aligned, sizeof(offset_aligned));
			f.read((char *)&length_aligned, sizeof(length_aligned));
			
			//Apply the logical shift alignment read earlier.
			int offset = (int)offset_aligned << lsa;
			int length = (int)length_aligned << lsa;
			
			//Read the resource ID.
			uint16_t id;
			f.seekg(2, ios::cur);
			f.read((char *)&id, sizeof(id));
			
			//Skip reserved bytes.
			f.seekg(4, ios::cur);
			
			//Store the resource.
			Resource & r = resources[type][id];
			r.type   = type;
			r.id     = id;
			r.offset = offset;
			r.length = length;
			r.data   = new char [r.length];
			
			//Read the resource data.
			size_t backup = f.tellg();
			f.seekg(r.offset, ios::beg);
			f.read(r.data, r.length);
			f.seekg(backup, ios::beg);
			if((size_t)f.tellg() == backup)	f.clear(ifstream::goodbit);	// Clears eofbit in cases when seeking from EOF
		}
	}
	return true;
	
	//In case of an unexpected eof, jump here.
	unexpected_eof:
	LOG(ERROR, "file is too short (unexpected eof after %i bytes)", (int)f.tellg());
	return false;
}

/** Dumps the loaded resources to the directory at path. */
void WindowsNEExecutable::dump(Path path)
{
	LOG(INFO, "dumping to %s", (const char *)path);
	
	//TODO: make sure this also works on windows. or better use FileSystem class once done.
	struct stat st;
	
	if (stat(path, &st) != 0) {
		LOG(DEBUG, "  creating directory");
		if (mkdir(path, 0777) != 0) {
			LOG(ERROR, "unable to make directory %s", (const char *)path);
			return;
		}
	}
	
	char temp[16];
	for (ResourceTable::iterator t = resources.begin(); t != resources.end(); t++) {
		snprintf(temp, 16, "%x", t->first);
		Path dir = path.down(temp);
		
		if (stat(dir, &st) != 0) {
			if (mkdir(dir, 0777) != 0) {
				LOG(ERROR, "unable to make directory %s", (const char *)dir);
				return;
			}
		} 
		
		for (Resources::iterator r = t->second.begin(); r != t->second.end(); r++) {
			snprintf(temp, 16, "%x.bin", r->second.id);
			Path p = dir.down(temp);
			
			ofstream f(p.c_str());
			f.write(r->second.data, r->second.length);
			f.close();
		}
	}
}
