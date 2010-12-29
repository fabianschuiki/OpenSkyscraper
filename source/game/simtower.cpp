#include "simtower.h"

#include "../core/platform.h"
#include "../resources/texture.h"

#include <sys/stat.h>


using namespace OSS;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Resource Names
//----------------------------------------------------------------------------------------------------

SimTower::ResourceName SimTower::resourceNames[] = {
	/**
	 * User Interface
	 */
	
	//Toolbox
	// -> play/pause
	{0x25A, "ui/toolbox/pause-N"},				//pause, normal
	{0x25B, "ui/toolbox/pause-P"},				//pause, pressed
	{0x258, "ui/toolbox/play-N"},				//play, normal
	{0x259, "ui/toolbox/play-P"},				//play. pressed
	// -> tools
	{0x25C, "ui/toolbox/tools-N"},				//normal
	{0x25D, "ui/toolbox/tools-S"},				//selected
	{0x25E, "ui/toolbox/tools-D"},				//disabled
	// -> construction
	{0x12C, "ui/toolbox/construction-N"},		//normal
	{0x12D, "ui/toolbox/construction-S"},		//selected
	{0x12E, "ui/toolbox/construction-D"},		//disabled
	
	
	/**
	 * Background
	 */
	
	//Ground
	{0x351, "background/ground"},
	
	//Sky
	{0x352, "background/sky0"},
	{0x353, "background/sky1"},
	{0x354, "background/sky2"},
	{0x355, "background/sky3"},
	{0x356, "background/sky4"},
	{0x357, "background/sky5"},
	{0x358, "background/sky6"},
	{0x359, "background/sky7"},
	{0x35A, "background/sky8"},
	{0x35B, "background/sky9"},
	
	//Clouds
	{0x384, "background/cloud0"},
	{0x385, "background/cloud1"},
	{0x386, "background/cloud2"},
	{0x387, "background/cloud3"},
	
	//City
	{0x389, "background/city"},
	
	
	/**
	 * Decoration
	 */
	
	//Tower
	{0x3E9, "decoration/entrance"},	//tower entrances (left and right)
	{0x3EA, "decoration/crane"},	//crane that sits ontop of the tower
	{0x42D, "decoration/stairs"},	//fire escape stairs
	
	//Santa
	{0x388, "decoration/santa"},
	
	{0, ""}
};

std::string SimTower::getNameForResource(Resource * resource)
{
	//Try to find a name in the resource names table
	for (int i = 0; SimTower::resourceNames[i].resourceID != 0; i++)
		if (SimTower::resourceNames[i].resourceID == resource->id)
			return SimTower::resourceNames[i].name;
	
	//None was found, so we simply return the resource ID in hex
	char name[16];
	sprintf(name, "%X", resource->id);
	return name;
}

std::string SimTower::Resource::getName()
{
	return SimTower::getNameForResource(this);
}

std::string SimTower::Resource::getDumpName()
{
	std::string name = getName();
	std::string::size_type pos = 0;
	while ((pos = name.find("/", pos)) != std::string::npos) {
		name.erase(pos, 1);
		name.insert(pos, ":");
	}
	return name;
}





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
	std::string dumpPath = "/tmp/SimTower Extraction/";
	mkdir(dumpPath.c_str(), 0777);
	dumpPath += "textures/";
	mkdir(dumpPath.c_str(), 0777);
	
	//Iterate through the resources
	for (ResourceVector::iterator i = resources.begin(); i != resources.end(); i++) {
		Resource * resource = (*i);
		
		//Treat the supported resource types
		switch (resource->type) {
				//Bitmap
			case 0x2: {
				//Create the BMP header
				struct {
					uint16_t type;
					uint32_t size;
					uint32_t _reserved;
					uint32_t offset;
				} __attribute__((__packed__)) bmpHeader = {0x4D42, 0, 0, 0x436};
				
				//Create a new buffer to hold the assembled data
				unsigned int bufferLength = sizeof(bmpHeader) + resource->length;
				uint8_t * buffer = (uint8_t *)malloc(bufferLength);
				memcpy(buffer, &bmpHeader, sizeof(bmpHeader));
				memcpy(buffer + sizeof(bmpHeader), resource->data, resource->length);
				
				//DEBUG: Dump the buffer for debuggin purposes
				FILE * fdump = fopen((dumpPath + resource->getDumpName() + ".bmp").c_str(), "w");
				fwrite(buffer, 1, bufferLength, fdump);
				fclose(fdump);
				
				//Assemble the texture name
				std::string textureName = "simtower/";
				textureName += resource->getName();
				
				//Create a texture from it
				OSS::Texture * texture = OSS::Texture::named(textureName);
				texture->assignLoadedData(IL_BMP, buffer, bufferLength);
				texture->useTransparencyColor = true;
				
				//Get rid of the buffer
				free(buffer);
			} break;
		}
	}
}

void SimTower::extractSounds()
{
}
