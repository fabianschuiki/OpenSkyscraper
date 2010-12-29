#include "simtower.h"

#include "../core/platform.h"


using namespace OSS;


//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Loading
//----------------------------------------------------------------------------------------------------

void SimTower::loadResources()
{
	//Get rid of the old resources
	resources.clear();
	
	//Get the path to the SimTower instance
	std::string path = Platform::shared()->pathToResource("SIMTOWER.EXE");
	
	//Open the file
	FILE * fsimtower = fopen(path.c_str(), "rb");
	if (!fsimtower) {
		OSSObjectError << "unable to open SimTower at " << path << std::endl;
		return;
	}
	
	//Seek forward to the segmented EXE header offset
	uint32_t shoffset;
	fseek(fsimtower, 0x3C, SEEK_SET);
	fread(&shoffset, 1, sizeof(shoffset), fsimtower);
	shoffset = SDL_SwapLE32(shoffset);
	
	//Read the resource table offset
	uint16_t rtoffset;
	fseek(fsimtower, shoffset + 0x24, SEEK_SET);
	fread(&rtoffset, 1, sizeof(rtoffset), fsimtower);
	rtoffset = SDL_SwapLE16(rtoffset);
	
	//Read the logical sector offset shift
	uint16_t lsashift;
	fseek(fsimtower, shoffset + 0x32, SEEK_SET);
	fread(&lsashift, 1, sizeof(lsashift), fsimtower);
	lsashift = SDL_SwapLE16(lsashift);
	
	//Seek forward to the resource table
	fseek(fsimtower, shoffset + rtoffset + 2, SEEK_SET);
	
	//Go through the resource table
	while (!feof(fsimtower)) {
		//Read the resource type and number of entries
		uint16_t resourceType, numberOfResources;
		fread(&resourceType, 1, sizeof(resourceType), fsimtower);
		fread(&numberOfResources, 1, sizeof(numberOfResources), fsimtower);
		resourceType = SDL_SwapLE16(resourceType) & 0x7FFF;
		numberOfResources = SDL_SwapLE16(numberOfResources);
		
		//A resource type of 0 marks the end of the list
		if (resourceType == 0)
			break;
		
		//Skip ahead to the first resource
		fseek(fsimtower, 4, SEEK_CUR);
		
		//Read the individual resources
		for (int i = 0; i < numberOfResources; i++) {
			//Read the offset and length
			uint16_t resourceOffset, resourceLength;
			fread(&resourceOffset, 1, sizeof(resourceOffset), fsimtower);
			fread(&resourceLength, 1, sizeof(resourceLength), fsimtower);
			resourceOffset = SDL_SwapLE16(resourceOffset);
			resourceLength = SDL_SwapLE16(resourceLength);
			
			//Read the resource ID
			uint16_t resourceID;
			fseek(fsimtower, 2, SEEK_CUR);
			fread(&resourceID, 1, sizeof(resourceID), fsimtower);
			resourceID = SDL_SwapLE16(resourceID) & 0x7FFF;
			
			//Skip reserved bytes
			fseek(fsimtower, 4, SEEK_CUR);
			
			//Calculate the actual offset and length in bytes
			unsigned int byteOffset = resourceOffset;
			unsigned int byteLength = resourceLength;
			byteOffset <<= lsashift;
			byteLength <<= lsashift;
			
			//Backup the current location in the executable
			long location = ftell(fsimtower);
			
			//Initialize memory for the resource
			void * buffer = malloc(byteLength);
			
			//Seek to the resource and read its data
			fseek(fsimtower, byteOffset, SEEK_SET);
			fread(buffer, 1, byteLength, fsimtower);
			
			//Restore the old location
			fseek(fsimtower, location, SEEK_SET);
			
			//Create a new resource instance
			Resource * resource = new Resource;
			resource->type = resourceType;
			resource->id = resourceID;
			resource->length = byteLength;
			resource->data = buffer;
			
			//Add the resource
			resources.push_back(resource);
		}
	}
	
	//Close the SimTower file
	fclose(fsimtower);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Extraction
//----------------------------------------------------------------------------------------------------

void SimTower::extractAll()
{
	extractTextures();
	extractSounds();
}

void SimTower::extractTextures()
{
}

void SimTower::extractSounds()
{
}
