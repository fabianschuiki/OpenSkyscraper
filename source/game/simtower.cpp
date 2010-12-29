#include "simtower.h"
#include "../classes.h"

#ifdef WIN32 /* why can't windows be the same as unix */
#include <direct.h>
#define mkdir(a) _mkdir(a)
#else
#include <sys/stat.h>
#define mkdir(a) mkdir(a, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#endif

using namespace OSS;


typedef struct {
	uint16_t length;
	uint16_t id;
	uint8_t * data;
} SimTowerResource;


void SimTower::reloadResources()
{
	std::string path = Platform::shared()->pathToResource("SIMTOWER.EXE");
	OSSObjectLog << path << std::endl;
	
	FILE * f = fopen(path.c_str(), "rb");
	if (!f) {
		OSSObjectError << "unable to open file" << std::endl;
		return;
	}
	
	std::string basePath = "/tmp/SimTower Extraction";
	mkdir(basePath.c_str());
	
	std::string soundPath = basePath + "/sounds";
	std::string imagePath = basePath + "/images";
	mkdir(soundPath.c_str());
	mkdir(imagePath.c_str());
	
	uint8_t buffer[0x200];
	long offset;
	int bitmapIndex = 0;
	int waveIndex = 0;
	while (1) {
		offset = ftell(f);
		if (fread(buffer, 1, 0x200, f) < 1) break;
		
		//Bitmap
		if (memcmp(buffer, (uint8_t []){0x28, 0x00, 0x00, 0x00}, 4) == 0) {
			//OSSObjectLog << "found BMP at " << offset << std::endl;
			
			//Extract the image length in bytes
			uint32_t imageLength = *(uint32_t *)(buffer + 20);
			
			//Create the file header
			struct {
				uint16_t type; //should be 'BM'
				uint32_t size;
				uint32_t _reserved;
				uint32_t offBits;
			} __attribute__((__packed__)) fileHeader = {0x4D42, 0, 0, 0x436};
			fileHeader.size = imageLength + fileHeader.offBits;
			
			//Allocate a buffer large enough to hold the entire BMP
			int bmpLength = fileHeader.size;
			uint8_t * bmp = (uint8_t *)malloc(bmpLength);
			
			//Copy the file header into the bmp
			memcpy(bmp, &fileHeader, sizeof(fileHeader));
			int bmpOffset = sizeof(fileHeader);
			
			//Copy the already read buffer into bmp
			memcpy(bmp + bmpOffset, buffer, 0x200);
			bmpOffset += 0x200;
			
			//Read the rest of the buffer
			fread(bmp + bmpOffset, 1, fileHeader.size - sizeof(fileHeader) - 0x200, f);
			fseek(f, offset + 0x200, SEEK_SET);
			
			
			//Create a texture resource from it
			char name[512]; sprintf(name, "simtower/%03i.bmp", bitmapIndex);
			/*OSS::Texture * texture =*/ //new OSS::Texture(name, IL_BMP, bmp, bmpLength);
			OSS::Texture::named(name)->assignLoadedData(IL_BMP, bmp, bmpLength);
			
			
			//Dump the BMP file
			char path[512]; sprintf(path, "%s/%03i.bmp", imagePath.c_str(), bitmapIndex++);
			FILE * dump = fopen(path, "w");
			fwrite(bmp, 1, bmpLength, dump);
			fclose(dump);
		}
		
		//Wave Sound
		if (memcmp(buffer, "RIFF", 4) == 0 && memcmp(buffer + 8, "WAVE", 4) == 0) {
			//OSSObjectLog << "found WAV at " << offset << std::endl;
			
			//Extract the length of the file
			uint32_t waveLength = *(uint32_t *)(buffer + 4);
			waveLength += 8;
			
			//Create a buffer capable of holding the entire file
			uint8_t * wave = (uint8_t *)malloc(waveLength);
			
			//Copy the laready read buffer
			memcpy(wave, buffer, 0x200);
			
			//Read the rest of the buffer
			fread(wave + 0x200, 1, waveLength - 0x200, f);
			fseek(f, offset + 0x200, SEEK_SET);
			
			
			//Dump the WAV file
			char path[512]; sprintf(path, "%s/%03i.wav", soundPath.c_str(), waveIndex++);
			FILE * dump = fopen(path, "w");
			fwrite(wave, 1, waveLength, dump);
			fclose(dump);
		}
	}
	
	fclose(f);
}
