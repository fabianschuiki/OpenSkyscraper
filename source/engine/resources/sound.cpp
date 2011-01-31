#include "sound.h"

using namespace OSS;


//Chunk Header
typedef struct {
	uint32_t id;
	uint32_t size;
	uint32_t format;
} __attribute__((__packed__)) chunkHeader;

//Subchunk Header
typedef struct {
	uint32_t id;
	uint32_t size;
} __attribute__((__packed__)) subchunkHeader;

//fmt Subchunk
typedef struct {
	uint16_t audioFormat;
	uint16_t numChannels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitsPerSample;
} __attribute__((__packed__)) fmtSubchunk;


//Store
SoundStore * Sound::store;


//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

Sound::Sound(string name) : StoreItem(name)
{
	bufferID = 0;
}

Sound::~Sound()
{
	//Delete the OpenAL buffer
	if (bufferID) {
		alDeleteBuffers(1, &bufferID);
		bufferID = 0;
	}
}

string Sound::instanceName()
{
	return this->className() + " " + name;
}

void Sound::assignLoadedData(const void * data, ALuint length)
{
	//Check whether the data is actually a RIFF file. Some versions of SimTower have resources with
	//the audio wave type ID, but not of the WAVE file format.
	if (memcmp(data, "RIFF", 4) != 0) {
		OSSObjectLog << "resource " << this->name << " is not a RIFF file." << std::endl;
		return;
	}
	
	//DEBUG: We do the parsing of the sound data here for the sake of simplicity. Should be moved to
	//the appropriate StoreItem functions below later on.
	//Find the WAVE chunk
	const chunkHeader * riffHeader = (chunkHeader *)data;
	while (riffHeader && SDL_SwapBE32(riffHeader->format) != 'WAVE')
		riffHeader = (chunkHeader *)((uint8_t *)riffHeader + riffHeader->size + sizeof(*riffHeader));
	if (!riffHeader) return;
	
	//Find the fmt subchunk
	const subchunkHeader * fmtHeader = (subchunkHeader *)((uint8_t *)riffHeader + sizeof(*riffHeader));
	while (fmtHeader && SDL_SwapBE32(fmtHeader->id) != 'fmt ')
		fmtHeader = (subchunkHeader *)((uint8_t *)fmtHeader + fmtHeader->size + sizeof(*fmtHeader));
	if (!fmtHeader) return;
	const fmtSubchunk * fmt = (fmtSubchunk *)((uint8_t *)fmtHeader + sizeof(*fmtHeader));
	
	//Find the data subchunk
	const subchunkHeader * dataHeader = (subchunkHeader *)((uint8_t *)riffHeader + sizeof(*riffHeader));
	while (dataHeader && SDL_SwapBE32(dataHeader->id) != 'data')
		dataHeader = (subchunkHeader *)((uint8_t *)dataHeader + dataHeader->size + sizeof(*dataHeader));
	if (!dataHeader) return;
	const void * samples = (uint8_t *)dataHeader + sizeof(*dataHeader);
	
	//Generate the buffer
	assert(!bufferID);
	alGenBuffers(1, &bufferID);
	
	//Load the buffer data
	ALenum format = (fmt->bitsPerSample == 16
					 ? (fmt->numChannels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16)
					 : (fmt->numChannels == 1 ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8));
	alBufferData(bufferID, format, samples, dataHeader->size, fmt->sampleRate);
	if (alGetError() != AL_NO_ERROR) {
		OSSObjectError << "unable to alBufferData!" << std::endl;
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Loading / Unloading
//----------------------------------------------------------------------------------------------------

void Sound::load()
{
}

void Sound::finalize()
{
}


void Sound::unfinalize()
{
}

void Sound::unload()
{
}
