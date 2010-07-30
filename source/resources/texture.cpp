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
	tempImage = 0;
}

Texture::~Texture()
{
	//Delete the OpenGL texture
	if (textureID) {
		glDeleteTextures(1, &textureID);
		textureID = 0;
	}
	
	//In case something went wrong during loading or the texture is released before being finalized,
	//we also have to get rid of the temporary image.
	if (tempImage) {
		ilDeleteImages(1, &tempImage);
		tempImage = 0;
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Loading / Unloading
//----------------------------------------------------------------------------------------------------

void Texture::load()
{
	//Assemble the path to the texture PNG
	std::string path = Platform::shared()->pathToResource("textures", name);
	OSSObjectLog << "Loading '" << path << "'..." << std::endl;
	
	//Create a new IL image
	assert(tempImage == 0);
	ilGenImages(1, &tempImage);
	ilBindImage(tempImage);
	
	//Load the image
	ILboolean success = ilLoadImage(path.c_str());
	if (success) {
		//Remember the texture size
		size.x = ilGetInteger(IL_IMAGE_WIDTH);
		size.y = ilGetInteger(IL_IMAGE_HEIGHT);
		
		//Convert the image to a OpenGL-appropriate format
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		if (!success)
			OSSObjectError << "unable to convert image " << tempImage << " to IL_RGBA and IL_UNSIGNED_BYTE" << std::endl;
	} else {
		OSSObjectError << "unable to load image via ilLoadImage() from " << path << std::endl;
	}
	
	//Unbind the image (to prevent any errors that might occur)
	ilBindImage(0);
	
	//Delete the image if anything went wrong
	if (!success) {
		ilDeleteImages(1, &tempImage);
		tempImage = 0;
	}
}

void Texture::finalize()
{
	assert(textureID == 0);
	OSSObjectLog << "Finalizing '" << name << "'..." << std::endl;
	
	//Only proceed if we have a valid image
	if (!tempImage) return;
	ilBindImage(tempImage);
	
	//Create the texture
	glGenTextures(1, &textureID);
	
	//Actually create the OpenGL texture from the temporary bitmap
	glBindTexture(GL_TEXTURE_RECTANGLE_EXT, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_RECTANGLE_EXT,
				 0,
				 GL_RGBA,
				 size.x,
				 size.y,
				 0,
				 ilGetInteger(IL_IMAGE_FORMAT),
				 GL_UNSIGNED_BYTE,
				 ilGetData());
	
	//Some texture parameters
	glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//Get rid of the temporary image
	ilBindImage(0);
	ilDeleteImages(1, &tempImage);
	tempImage = 0;
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
#pragma mark Binding
//----------------------------------------------------------------------------------------------------

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_RECTANGLE_EXT, textureID);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_RECTANGLE_EXT, 0);
}
