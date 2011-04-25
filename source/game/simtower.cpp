#include "simtower.h"

#include <sys/stat.h>
extern "C" {
	#include <mspack.h>
}

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


const color3d SimTower::skyThroughWindow[] = {
	(color3d){140 / 255.0, 214 / 255.0, 1.0},
	(color3d){66 / 255.0, 198 / 255.0, 1.0},
	(color3d){74 / 255.0, 180 / 255.0, 1.0}
};





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

unsigned int SimTower::kPercellBitmapResource = 0x7F02;
unsigned int SimTower::kSoundResource = 0x7F0A;
unsigned int SimTower::kReplacePaletteResource = 0x7F03;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Resource Names
//----------------------------------------------------------------------------------------------------

string SimTower::getNameForResource(Resource * resource)
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
	snprintf(name, 16, "#%X", resource->id);
	return name;
}

string SimTower::Resource::getName()
{
	return SimTower::getNameForResource(this);
}

string SimTower::Resource::getDumpName()
{
	string name = getName();
	string::size_type pos = 0;
	while ((pos = name.find("/", pos)) != string::npos) {
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
	string path = application->pathToResource("SIMTOWER.EXE");
	
	//If there is no file at the specified path, check whether there is a SIMTOWER.EX_ file.
	if (path == "") {
		
		//Decide where to decompress the file.
		path = "/tmp/SIMTOWER.EXE";
		
		//Find the compressed version.
		string compressed = application->pathToResource("SIMTOWER.EX_");
		std::cout << "decompressing " << compressed << " to " << path << " ";
		
		//Decompress the file.
		mskwaj_decompressor * decompressor = mspack_create_kwaj_decompressor(NULL);
		decompressor->decompress(decompressor, (char *)compressed.c_str(), (char *)path.c_str());
		mspack_destroy_kwaj_decompressor(decompressor);
		std::cout << "done" << std::endl;
	}
	
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
		resourceType = SDL_SwapLE16(resourceType);
		numberOfResources = SDL_SwapLE16(numberOfResources);
		
		//If this is a non-standard resource, we have to lookup its actual name so we're able to
		//dynamically assign the type IDs of the percell bitmaps, replacement palettes and sound re-
		//sources.
		if (!(resourceType & 0x8000)) {
			//Keep the current position in the tower file around for later use.
			long pos = ftell(fsimtower);
			
			//Seek to the position where the type name of this resource is stored.
			fseek(fsimtower, shoffset + rtoffset + resourceType, SEEK_SET);
			
			//Read the length and the type itself.
			char length;
			fread(&length, 1, sizeof(char), fsimtower);
			char *buf = new char[length + 1];
			fread(buf, length, sizeof(char), fsimtower);
			buf[length] = 0;
			
			//Restore the previous position in the tower file.
			fseek(fsimtower, pos, SEEK_SET);
			
			//Based on the name we now may assign the resource type to its corresponding variable.
			if (!strcmp(buf, "CGPK"))
				kPercellBitmapResource = resourceType;
			else if (!strcmp(buf, "CLUT"))
				kReplacePaletteResource = resourceType;
			else if (!strcmp(buf, "WAVE"))
				kSoundResource = resourceType;
			delete [] buf;
		}
		
		//Since this is a standard resource, all we have to do is get rid of the flag which indi-
		//cated this fact.
		else {
			resourceType &= 0x7FFF;
		}
		
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
		if (resource->type == kBitmapResource) {
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
		} else if (resource->type == kPercellBitmapResource) {
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
			Resource * paletteResource = getResource(kReplacePaletteResource, 0x3E8);
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
		} else if (resource->type == kReplacePaletteResource) {	
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
		}
	}
}

void SimTower::extractSounds()
{	
	//Iterate through the resources
	for (ResourceVector::iterator i = resources.begin(); i != resources.end(); i++) {
		Resource * resource = (*i);
		
		//Treat the supported resource types
		if (resource->type == kSoundResource) {
			/*//DEBUG: Dump the buffer for debugging purposes
			FILE * fdump = fopen((dumpPath + resource->getDumpName() + ".wav").c_str(), "w");
			fwrite(resource->data, 1, resource->length, fdump);
			fclose(fdump);*/
			
			//Assemble the sound name
			string soundName = "simtower/";
			soundName += resource->getName();

			//Dump the sound
			dumpSound(soundName, resource->data, resource->length);
			
			//Create a sound from it
			Sound * sound = Sound::named(soundName);
			sound->assignLoadedData(resource->data, resource->length);
		}
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Postprocessing
//----------------------------------------------------------------------------------------------------

void SimTower::postprocessTexture(string resourceName,
								  const void * buffer, unsigned int bufferLength)
{
	//Assemble the texture name
	string texturePrefix("simtower/");
	string textureName = texturePrefix + resourceName;
	
	//Load the image
	ILuint image = ilGenImage();
	ilBindImage(image);
	ilLoadL(IL_BMP, buffer, bufferLength);
	bool processed = false;
	
	//Sky textures
	if (resourceName.find("background/sky") == 0) {
		spawnSkyTextures(textureName, image);
		processed = true;
	}
	
	//Cloud textures
	if (resourceName.find("background/cloud") == 0) {
		spawnCloudTextures(textureName, image);
		processed = true;
	}
	
	//Single lobby textures
	if (resourceName.find("lobby/single") == 0) {
		spawnLobbyTextures(textureName, image);
		processed = true;
	}
	
	//Elevator textures
	if (resourceName.find("transport/elevator") == 0) {
		spawnElevatorTextures(textureName, image);
		processed = true;
	}
	
	//Floordigit textures
	if (resourceName.find("transport/floordigits") == 0) {
		spawnFloordigitTextures(textureName, image);
		processed = true;
	}
	
	//Get rid of the image
	ilBindImage(0);
	ilDeleteImage(image);
	if (processed)
		return;
	
	//Standard textures
	Texture * texture = Texture::named(textureName);
	texture->assignLoadedData(IL_BMP, buffer, bufferLength);
	texture->useTransparentColor = true;
	dumpTexture(texture);
	
	//Setup the correct transparency behaviour
	if (resourceName == "background/city")
		texture->transparentColor = (color3d){138 / 255.0, 212 / 255.0, 1.0};
	if (resourceName.find("facilities") == 0) {
		texture->transparentColors.push_back(skyThroughWindow[0]);
		texture->transparentColors.push_back(skyThroughWindow[1]);
		texture->transparentColors.push_back(skyThroughWindow[2]);
	}
	if (resourceName.find("ui") == 0 &&
		resourceName.find("ui/control/star") != 0)
		texture->useTransparentColor = false;
}

void SimTower::applyReplacementPalette(unsigned short id)
{
	//Fetch the resource containing the replacement palette
	Resource * replacementPalette = getResource(kReplacePaletteResource, id);
	if (!replacementPalette) {
		OSSObjectLog << "unable to get replacement palette " << id << std::endl;
		return;
	}
	
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

void SimTower::spawnSkyTextures(string textureName, ILuint image)
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
	
	//Create the rain textures if this is is a sky slice and not a cloud
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

void SimTower::spawnCloudTextures(string textureName, ILuint image)
{
	struct {
		string name;
		unsigned int palette;
		Texture * texture;
	} variants[4] = {
		{"day", 0x3E8},
		{"overcast", 0x3EB},
		{"twilight", 0x3E9},
		{"night", 0x3EA}
	};
	
	//Load the variants
	for (unsigned int i = 0; i < 4; i++) {
		applyReplacementPalette(variants[i].palette);
		Texture * texture = Texture::named(textureName + "/" + variants[i].name);
		texture->assignLoadedImage(ilCloneCurImage());
		texture->useTransparentColor = true;
		variants[i].texture = texture;
	}
	
	//Dump the textures
	for (unsigned int i = 0; i < 4; i++)
		dumpTexture(variants[i].texture);
}

void SimTower::spawnLobbyTextures(string textureName, ILuint image)
{	
	//Convert the lobby image to 24 bit RGB
	if (!ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE)) {
		OSSObjectError << "unable to convert image " << image << " of " << textureName << " to 8bit RGB" << std::endl;
		return;
	}
	
	//Load the two variants of the lobby (ground and sky)
	Texture * t;
	string variants[2] = {"ground", "sky"};
	for (int i = 0; i < 2; i++) {
		//Assemble the texture name without prefix
		string variantName = textureName + "/" + variants[i];
		
		//Extract the lobby pattern
		ILuint pattern = ilGenImage();
		ilBindImage(pattern);
		ilTexImage(256, 36, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, NULL);
		ilBlit(image, 0, 0, 0, i * 328, 0, 0, 256, 36, 1);
		t = Texture::named(variantName + "/pattern");
		t->assignLoadedImage(pattern);
		t->transparentColors.push_back(skyThroughWindow[0]);
		t->transparentColors.push_back(skyThroughWindow[1]);
		t->useTransparentColor = true;
		dumpTexture(t);
		
		//Extract the lobby entrance
		ILuint entrance = ilGenImage();
		ilBindImage(entrance);
		ilTexImage(56, 36, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, NULL);
		ilBlit(image, 0, 0, 0, i * 328 + 272, 0, 0, 56, 36, 1);
		t = Texture::named(variantName + "/entrance");
		t->assignLoadedImage(entrance);
		t->transparentColors.push_back(skyThroughWindow[0]);
		t->transparentColors.push_back(skyThroughWindow[1]);
		t->useTransparentColor = true;
		dumpTexture(t);
	}
}

void SimTower::spawnElevatorTextures(string textureName, ILuint image)
{
	string prefix("transport/elevator/");
	string subname = textureName.substr(textureName.find(prefix) + prefix.length());
	
	//Calculate the cell width of this elevator
	unsigned int cellWidth = (subname.find("express") == 0 ? 6 : 4);
	
	//Extract the image size
	unsigned int width = ilGetInteger(IL_IMAGE_WIDTH);
	unsigned int height = ilGetInteger(IL_IMAGE_HEIGHT);
	
	//If this is either the standard or express elevator, cut off the motor and buffer
	if (subname == "standard" || subname == "express") {
		//Decide what to cut
		int cutWidth = 2 * 8 * cellWidth;
		int cutX = width - cutWidth;
		
		//The motor has 3 animation frames
		for (int frame = 0; frame < 3; frame++) {
			ilBindImage(image);
			
			//Rotate the animation colors
			unsigned int dataSize = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
			ILubyte * data = ilGetData();
			for (int i = 0; i < dataSize; i++) {
				if (data[i] >= 201 && data[i] <= 203)
					data[i] = ((data[i] - 201 + 1) % 3 + 201);
				if (data[i] >= 197 && data[i] <= 198)
					data[i] = ((data[i] - 197 + 1) % 2 + 197);
			}
			
			//Copy the pixels
			ILubyte * pixels = (ILubyte *)malloc(cutWidth * height * 3);
			ilCopyPixels(cutX, 0, 0, cutWidth, height, 1, IL_RGB, IL_UNSIGNED_BYTE, pixels);
		
			//Create a new image to hold the slice
			ILuint slice = ilGenImage();
			ilBindImage(slice);
			ilTexImage(cutWidth, height, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, NULL);
			
			//Copy the pixels into the slice
			ilSetPixels(0, 0, 0, cutWidth, height, 1, IL_RGB, IL_UNSIGNED_BYTE, pixels);
			free(pixels);
			
			//Create a texture from the slice
			stringstream frameName;
			frameName << "/motorbuffer/" << frame;
			Texture * t = Texture::named(textureName + frameName.str());
			t->assignLoadedImage(slice);
			dumpTexture(t);
		}
		
		//Covnert the image to RGB
		ilBindImage(image);
		ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
		
		//Create a new image to hold the remaining image
		ILuint remainder = ilGenImage();
		ilBindImage(remainder);
		ilTexImage(width - cutWidth, height, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, NULL);
		
		//Blit the remaining part of the image into this one
		ilBlit(image, 0, 0, 0, 0, 0, 0, width - cutWidth, height, 1);
		
		//Recall this function with the remaining image
		if (subname == "standard")
			spawnElevatorTextures(textureName + "/car/occupied", remainder);
		else
			spawnElevatorTextures(textureName + "/car", remainder);
		
		//Get rid of the remainder
		ilDeleteImage(remainder);
		return;
	}
	
	//Convert the source image to an appropriate format
	ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
	
	//If this is either the service or express elevator, cut off the empty car
	if (subname == "service/car" || subname == "express/car") {
		//Decide what to cut
		int cutWidth = 8 * cellWidth;
		
		//Create a new image with the empty car
		ILuint empty = ilGenImage();
		ilBindImage(empty);
		ilTexImage(cutWidth, height, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, NULL);
		ilBlit(image, 0, 0, 0, 0, 0, 0, cutWidth, height, 1);
		
		//Create a texture from the empty car
		Texture * t = Texture::named(textureName + "/empty");
		t->assignLoadedImage(empty);
		dumpTexture(t);
		
		//Create a new image with the remaining image
		ILuint remainder = ilGenImage();
		ilBindImage(remainder);
		ilTexImage(width - cutWidth, height, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, NULL);
		ilBlit(image, 0, 0, 0, cutWidth, 0, 0, width - cutWidth, height, 1);
		
		//Recall this function with the remaining image
		spawnElevatorTextures(textureName + "/occupied", remainder);
		
		//Get rid of the remainder
		ilDeleteImage(remainder);
		return;
	}
	
	//For the rest of the images, simply clone the image and create a texture from it
	Texture * t = Texture::named(textureName);
	t->assignLoadedImage(ilCloneCurImage());
	dumpTexture(t);
}

void SimTower::spawnFloordigitTextures(string textureName, ILuint image)
{
	//Make all pixels white except for a few which make up the digits
	ilBindImage(image);
	unsigned int dataSize = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
	unsigned int width = ilGetInteger(IL_IMAGE_WIDTH);
	bool leaveLeftmostIntact = (textureName.find("/one/") != string::npos);
	ILubyte * data = ilGetData();
	for (int i = 0; i < dataSize; i++) {
		if (leaveLeftmostIntact && (i % width) < 32)
			continue;
		if (data[i] != 14 && data[i] != 78)
			data[i] = 0;
	}
	
	//Clone the image and create a texture from it
	Texture * t = Texture::named(textureName);
	t->assignLoadedImage(ilCloneCurImage());
	t->useTransparentColor = true;
	dumpTexture(t);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Debugging
//----------------------------------------------------------------------------------------------------

string SimTower::getDumpPath(string type, string name)
{
	string path("/tmp/SimTower Extraction/");
	mkdir(path.c_str(), 0777);
	path += type + "/";
	mkdir(path.c_str(), 0777);
	
	string::size_type pos = 0;
	while ((pos = name.find("/", pos)) != string::npos) {
		name.erase(pos, 1);
		name.insert(pos, ":");
	}
	path += name;
	
	return path;
}

void SimTower::dumpTexture(Texture * texture)
{
	if (!texture) return;
	ilBindImage(texture->tempImage);
	ilSaveImage(getDumpPath("textures", texture->name.substr(9) + ".bmp").c_str());
}

void SimTower::dumpSound(string name, const void * buffer, unsigned int bufferLength)
{
	FILE * fdump = fopen(getDumpPath("sounds", name.substr(9) + ".wav").c_str(), "w");
	fwrite(buffer, bufferLength, 1, fdump);
	fclose(fdump);
}
