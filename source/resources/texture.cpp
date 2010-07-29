#include "texture.h"
#include "../classes.h"

using namespace OSS;


//Store
TextureStore * Texture::store;

//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

Texture::Texture(std::string name) : StoreItem(name)
{
	textureID = 0;
}

Texture::~Texture()
{
	if (textureID) {
		glDeleteTextures(1, &textureID);
		textureID = 0;
	}
	temporaryBitmap = NULL;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Loading / Unloading
//----------------------------------------------------------------------------------------------------

void Texture::load()
{
	//Assemble the path to the texture PNG
	std::string path = Platform::shared()->pathToResource("textures", name, "png");
	OSSObjectLog << "Loading '" << path << "'..." << std::endl;
	
	//Read the bitmap
	temporaryBitmap = new Bitmap(path);
	size = temporaryBitmap->size;
}

void Texture::finalize()
{
	assert(textureID == 0);
	OSSObjectLog << "Finalizing '" << name << "'..." << std::endl;
	
	//Create the texture
	glGenTextures(1, &textureID);
	
	//Actually create the OpenGL texture from the temporary bitmap
	glBindTexture(GL_TEXTURE_RECTANGLE_EXT, textureID);
	glTexImage2D(GL_TEXTURE_RECTANGLE_EXT, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, temporaryBitmap->data);
	
	//Some texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//Get rid of the temporary bitmap
	temporaryBitmap = NULL;
}


void Texture::unfinalize()
{
	//Clear the OpenGL texture
	glDeleteTextures(1, &textureID);
	textureID = 0;
}

void Texture::unload()
{
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Texture
//----------------------------------------------------------------------------------------------------

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_RECTANGLE_EXT, textureID);
}
