#include "simtower.h"

#include "../core/platform.h"

#include <sys/stat.h>

using namespace OSS;


/**
 * Color structure used for palette modification.
 */

typedef union {
	struct {
		ILubyte b;
		ILubyte g;
		ILubyte r;
		ILubyte _x;
	} c;
	ILubyte v[4];
} colorPalette;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Resources
//----------------------------------------------------------------------------------------------------

SimTower::Resource * SimTower::getResource(unsigned short type, unsigned short id)
{
	for (ResourceVector::iterator it = resources.begin(); it != resources.end(); it++)
		if ((*it)->type == type && (*it)->id == id)
			return (*it);
	return NULL;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Resource Names
//----------------------------------------------------------------------------------------------------

std::string SimTower::getNameForResource(Resource * resource)
{
	if (!resource) return "";
	
	//Try to find a name in the resource names table
	// -> bitmaps
	if (resource->type == kBitmapResource || resource->type == kPercellBitmapResource)
		for (int i = 0; SimTower::bitmapResourceNames[i].resourceID != 0; i++)
			if (SimTower::bitmapResourceNames[i].resourceID == resource->id)
				return SimTower::bitmapResourceNames[i].name;
	// -> sounds
	if (resource->type == kSoundResource)
		for (int i = 0; SimTower::soundResourceNames[i].resourceID != 0; i++)
			if (SimTower::soundResourceNames[i].resourceID == resource->id)
				return SimTower::soundResourceNames[i].name;
	
	//None was found, so we simply return the resource ID in hex
	char name[16];
	sprintf(name, "#%X", resource->id);
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
				
				//Postprocess the bitmap
				postprocessTexture(resource->getName(), buffer, bufferLength);
				
				//Get rid of the buffer
				free(buffer);
			} break;
				
				//Per-cell Bitmaps
			case 0x7F02: {
				//Create the BMP header
				struct {
					uint16_t type;
					uint32_t size;
					uint32_t _reserved;
					uint32_t offset;
				} __attribute__((__packed__)) bmpHeader = {0x4D42, 0, 0, 0x436};
				
				//Calculate the image dimensions
				const unsigned int floorHeight = 36;
				const unsigned int cellWidth = 8;
				const unsigned int cellPixels = floorHeight * cellWidth;
				unsigned int cellsInResource = resource->length / cellPixels;
				
				//Create the DIB header
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
				} __attribute__((__packed__)) dibHeader = {
					40,
					(cellsInResource * cellWidth), floorHeight,
					1, 8,
					0,
					(cellsInResource * cellPixels),
					0, 0, 256, 256
				};
				
				//Create a new buffer to hold the assembled data
				unsigned int bufferLength = sizeof(bmpHeader) + sizeof(dibHeader) + 0x400 + dibHeader.imageSize;
				uint8_t * buffer = (uint8_t *)malloc(bufferLength);
				
				//Copy the bmpHeader into the buffer
				memcpy(buffer, &bmpHeader, sizeof(bmpHeader));
				
				//Copy the dibHeader into the buffer
				memcpy(buffer + sizeof(bmpHeader), &dibHeader, sizeof(dibHeader));
				
				//Assemble the palette
				uint8_t * palette = (buffer + sizeof(bmpHeader) + sizeof(dibHeader));
				Resource * paletteResource = getResource(0x7F03, 0x3E8);
				uint8_t * paletteSource = (uint8_t *)paletteResource->data;
				for (int i = 0; i < paletteResource->length; i++) {
					(*palette++) = paletteSource[i * 8 + 6];
					(*palette++) = paletteSource[i * 8 + 4];
					(*palette++) = paletteSource[i * 8 + 2];
					(*palette++) = 0;
				}
				
				//Since the actualy pixel data is only 8 pixels wide, with the individual cells
				//stacked vertically, we re-organize the buffer by flipping the image vertically and
				//lining up the cells horizontally to simplify postprocessing.
				uint8_t * src = (uint8_t *)resource->data;
				uint8_t * dst = buffer + bmpHeader.offset;
				for (unsigned int i = 0; i < dibHeader.imageSize; i++) {
					unsigned int srcx = (i % 8);
					unsigned int srcy = (i / 8);
					unsigned int dstx = srcx + (srcy / floorHeight) * 8;
					unsigned int dsty = floorHeight - 1 - (srcy % floorHeight);
					unsigned int dstidx = (dstx + dsty * dibHeader.width);
					dst[dstidx] = src[i];
				}
				
				//Postprocess the bitmap
				postprocessTexture(resource->getName(), buffer, bufferLength);
				
				//Get rid of the buffer
				free(buffer);
			} break;
				
				//DEBUG: Replacement Palettes
			case 0x7F03: {
				FILE * fdump = fopen(getDumpPath("palettes", resource->getName() + ".act").c_str(), "w");
				for (int i = 0; i < 256; i++) {
					//Calculate the location of the replacement color
					unsigned int replacementIndex = i;
					
					//For some reason, the color table has a duplicate entry for the color 184
					if (replacementIndex >= 184)
						replacementIndex++;
					
					//Since the replacement color palette still has 256 colors and compensating for the
					//duplicate entry will access the inexistent index 256 of the palette, we have to wrap
					//around at the end of the palette.
					replacementIndex = replacementIndex % 256;
					
					//Extract the replacement color from the palette. The replacement palettes are organized
					//as AARRGGBB, where the first byte of each channel corresponds to the value we want, and
					//the second byte to some other color whose use I haven't found out yet.
					ILubyte color[4];
					for (int n = 0; n < 4; n++)
						color[n] = ((ILubyte *)resource->data)[replacementIndex * 8 + (n * 2)];
					fwrite(color + 1, 1, 3, fdump);
				}
				fclose(fdump);
			} break;
		}
	}
}

void SimTower::extractSounds()
{	
	//Iterate through the resources
	for (ResourceVector::iterator i = resources.begin(); i != resources.end(); i++) {
		Resource * resource = (*i);
		
		//Treat the supported resource types
		switch (resource->type) {
				//Wave Audio
			case 0x7F0A: {
				/*//DEBUG: Dump the buffer for debugging purposes
				FILE * fdump = fopen((dumpPath + resource->getDumpName() + ".wav").c_str(), "w");
				fwrite(resource->data, 1, resource->length, fdump);
				fclose(fdump);*/
				
				//Assemble the sound name
				std::string soundName = "simtower/";
				soundName += resource->getName();
				
				//Dump the sound
				dumpSound(soundName, resource->data, resource->length);
				
				//Create a sound from it
				Sound * sound = Sound::named(soundName);
				sound->assignLoadedData(resource->data, resource->length);
			} break;
		}
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Postprocessing
//----------------------------------------------------------------------------------------------------

void SimTower::postprocessTexture(std::string resourceName,
								  const void * buffer, unsigned int bufferLength)
{
	//Assemble the texture name
	std::string texturePrefix("simtower/");
	std::string textureName = texturePrefix + resourceName;
	
	//Sky textures
	if (resourceName.find("background/sky") == 0) {
		//Load the image
		ILuint sky = ilGenImage();
		ilBindImage(sky);
		ilLoadL(IL_BMP, buffer, bufferLength);
		
		//Do the actual postprocessing
		spawnSkyTextures(textureName, sky);
		
		//Get rid of the image
		ilBindImage(0);
		ilDeleteImage(sky);
		return;
	}
	
	//Single lobby textures
	if (resourceName.find("lobby/single") == 0) {
		//Load the image
		ILuint lobby = ilGenImage();
		ilBindImage(lobby);
		ilLoadL(IL_BMP, buffer, bufferLength);
		
		//Split up the image
		spawnLobbyTextures(textureName, lobby);
		
		//Get rid of the image
		ilBindImage(0);
		ilDeleteImage(lobby);
		return;
	}
	
	//Standard textures
	Texture * texture = Texture::named(textureName);
	texture->assignLoadedData(IL_BMP, buffer, bufferLength);
	texture->useTransparentColor = true;
	dumpTexture(texture);
	
	//Some default colors for later use
	const color3d skyThroughWindow[2] = {
		(color3d){140 / 255.0, 214 / 255.0, 1.0},
		(color3d){66 / 255.0, 198 / 255.0, 1.0}
	};
	
	//Setup the correct transparency behaviour
	if (resourceName == "background/city")
		texture->transparentColor = (color3d){138 / 255.0, 212 / 255.0, 1.0};
	if (resourceName.find("facilities") == 0) {
		texture->transparentColors.push_back(skyThroughWindow[0]);
		texture->transparentColors.push_back(skyThroughWindow[1]);
	}
	if (resourceName.find("ui") == 0 &&
		resourceName.find("ui/control/star") != 0)
		texture->useTransparentColor = false;
}

void SimTower::applyReplacementPalette(unsigned short id)
{
	//Fetch the resource containing the replacement palette
	Resource * replacementPalette = getResource(0x7F03, id);
	
	//Fetch a pointer to the current image's palette
	colorPalette * palette = (colorPalette *)ilGetPalette();
	if (!palette) return;
	for (int i = 0; i < 256; i++) {
		//Calculate the location of the replacement color
		unsigned int replacementIndex = i;
		
		//For some reason, the color table has a duplicate entry for the color 184
		if (replacementIndex >= 184)
			replacementIndex++;
		
		//Since the replacement color palette still has 256 colors and compensating for the
		//duplicate entry will access the inexistent index 256 of the palette, we have to wrap
		//around at the end of the palette.
		replacementIndex = replacementIndex % 256;
		
		//Extract the replacement color from the palette. The replacement palettes are organized
		//as AARRGGBB, where the first byte of each channel corresponds to the value we want, and
		//the second byte to some other color whose use I haven't found out yet.
		colorPalette color;
		for (int n = 0; n < 4; n++)
			color.v[3 - n] = ((ILubyte *)replacementPalette->data)[replacementIndex * 8 + (n * 2)];
		palette[i] = color;
	}
}

void SimTower::spawnSkyTextures(std::string textureName, ILuint image)
{	
	//Fetch the palette
	colorPalette * palette = (colorPalette *)ilGetPalette();
	
	//Extract the color locations
	colorPalette * sky = &palette[188];
	colorPalette * darkDrops = &palette[207];
	colorPalette * brightDrops = &palette[213];
	
	//Extract the colors
	colorPalette darkColors[6]; memcpy(darkColors, darkDrops, sizeof(colorPalette) * 6);
	colorPalette brightColors[6]; memcpy(brightColors, brightDrops, sizeof(colorPalette) * 6);
	colorPalette nightColors[6]; memcpy(nightColors, &palette[173], sizeof(colorPalette) * 6);
	
	//Create the day texture
	for (int i = 0; i < 6; i++)
		sky[i] = darkDrops[i] = brightDrops[i] = brightColors[i];
	Texture * day = Texture::named(textureName + "/day");
	day->assignLoadedImage(ilCloneCurImage());
	
	//Create the overcast texture
	for (int i = 0; i < 6; i++)
		sky[i] = darkDrops[i] = brightDrops[i] = darkColors[i];
	Texture * overcast = Texture::named(textureName + "/overcast");
	overcast->assignLoadedImage(ilCloneCurImage());
	
	//Create the rain textures
	for (int i = 0; i < 6; i++) {
		sky[i] = brightDrops[i] = darkColors[i];
		darkDrops[i] = brightColors[i];
	}
	Texture * rain0 = Texture::named(textureName + "/rain/0");
	rain0->assignLoadedImage(ilCloneCurImage());
	for (int i = 0; i < 6; i++) {
		sky[i] = darkDrops[i] = darkColors[i];
		brightDrops[i] = brightColors[i];
	}
	Texture * rain1 = Texture::named(textureName + "/rain/1");
	rain1->assignLoadedImage(ilCloneCurImage());
	
	//Create the twilight texture
	applyReplacementPalette(0x3E9);
	for (int i = 0; i < 6; i++)
		darkDrops[i] = brightDrops[i] = sky[i];
	Texture * twilight = Texture::named(textureName + "/twilight");
	twilight->assignLoadedImage(ilCloneCurImage());
	
	//Create the night texture
	applyReplacementPalette(0x3EA);
	for (int i = 0; i < 6; i++)
		darkDrops[i] = brightDrops[i] = sky[i];
	Texture * night = Texture::named(textureName + "/night");
	night->assignLoadedImage(ilCloneCurImage());
	
	//Dump the textures
	dumpTexture(day);
	dumpTexture(overcast);
	dumpTexture(rain0);
	dumpTexture(rain1);
	dumpTexture(twilight);
	dumpTexture(night);
}

void SimTower::spawnLobbyTextures(std::string textureName, ILuint image)
{	
	//Convert the lobby image to 24 bit RGB
	if (!ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE)) {
		OSSObjectError << "unable to convert image " << image << " of " << textureName << " to 8bit RGB" << std::endl;
		return;
	}
	
	//Load the two variants of the lobby (ground and sky)
	Texture * t;
	std::string variants[2] = {"ground", "sky"};
	for (int i = 0; i < 2; i++) {
		//Assemble the texture name without prefix
		std::string variantName = textureName + "/" + variants[i];
		
		//Extract the lobby pattern
		ILuint pattern = ilGenImage();
		ilBindImage(pattern);
		ilTexImage(256, 36, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, NULL);
		ilBlit(image, 0, 0, 0, i * 328, 0, 0, 256, 36, 1);
		t = Texture::named(variantName + "/pattern");
		t->assignLoadedImage(pattern);
		dumpTexture(t);
		
		//Extract the lobby entrance
		ILuint entrance = ilGenImage();
		ilBindImage(entrance);
		ilTexImage(56, 36, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, NULL);
		ilBlit(image, 0, 0, 0, i * 328 + 272, 0, 0, 56, 36, 1);
		t = Texture::named(variantName + "/entrance");
		t->assignLoadedImage(entrance);
		dumpTexture(t);
	}
}

std::string SimTower::getDumpPath(std::string type, std::string name)
{
	std::string path("/tmp/SimTower Extraction/");
	mkdir(path.c_str(), 0777);
	path += type + "/";
	mkdir(path.c_str(), 0777);
	
	std::string::size_type pos = 0;
	while ((pos = name.find("/", pos)) != std::string::npos) {
		name.erase(pos, 1);
		name.insert(pos, ":");
	}
	path += name;
	
	return path;
}

void SimTower::dumpTexture(Texture * texture)
{
	ilBindImage(texture->tempImage);
	ilSaveImage(getDumpPath("textures", texture->name.substr(9) + ".bmp").c_str());
}

void SimTower::dumpSound(std::string name, const void * buffer, unsigned int bufferLength)
{
	FILE * fdump = fopen(getDumpPath("sounds", name.substr(9) + ".wav").c_str(), "w");
	fwrite(buffer, bufferLength, 1, fdump);
	fclose(fdump);
}
