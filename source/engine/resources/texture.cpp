#include "texture.h"

#include "../application.h"

using namespace OSS;


//Store
TextureStore * Texture::store;


//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

/**
 * Invoked by the TextureStore automatically, do not call on your own. Use Texture::named to obtain
 * a named texture instance for modification. Lazy loading breaks upon violation of this pattern!
 */
Texture::Texture(string name) : StoreItem(name)
{
	textureID = 0;
	tempImage = 0;
	useTransparentColor = false;
	transparentColor = (color3d){1, 1, 1};
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

string Texture::instanceName()
{
	return this->className() + " " + name;
}

void Texture::assignLoadedData(ILenum type, const void * data, ILuint length)
{
	if (tempImage) ilDeleteImage(tempImage);
	
	//Generate the temporary image
	tempImage = ilGenImage();
	ilBindImage(tempImage);
	
	//Load the image from the data
	ilLoadL(type, data, length);
	ilBindImage(0);
}

void Texture::assignLoadedImage(ILuint image)
{
	if (tempImage) ilDeleteImage(tempImage);
	tempImage = image;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Loading / Unloading
//----------------------------------------------------------------------------------------------------

void Texture::load()
{
	//If the temporary image doesn't exist already (supplied by the constructor), load it from disk
	ILboolean success = false;
	if (!tempImage) {
		//Assemble the path to the texture PNG
		string path = Application::shared()->pathToResource("textures", name);
		//OSSObjectLog << "Loading '" << path << "'..." << std::endl;
		
		//Create a new IL image
		tempImage = ilGenImage();
		ilBindImage(tempImage);
		
		//Load the image
		success = ilLoadImage(path.c_str());
		if (!success)
			OSSObjectError << "unable to load image via ilLoadImage() from " << path << std::endl;
	}
	
	//Otherwise just bind the temp image we already have
	else {
		ilBindImage(tempImage);
		success = true;
	}
	
	//Process the image
	if (success) {
		//Remember the texture size
		size.x = ilGetInteger(IL_IMAGE_WIDTH);
		size.y = ilGetInteger(IL_IMAGE_HEIGHT);
		
		//Convert the image to a OpenGL-appropriate format
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		if (!success)
			OSSObjectError << "unable to convert image " << tempImage << " to IL_RGBA and IL_UNSIGNED_BYTE" << std::endl;
		else {
			//Post-process the image (apply transparency based on a color)
			if (useTransparentColor) {
				ILubyte * imageData = ilGetData();
				for (unsigned int i = 0; i < ilGetInteger(IL_IMAGE_SIZE_OF_DATA); i += ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL)) {
					if (abs(imageData[i + 0] - transparentColor.c.r * 255) <= 1 &&
						abs(imageData[i + 1] - transparentColor.c.g * 255) <= 1 &&
						abs(imageData[i + 2] - transparentColor.c.b * 255) <= 1) {
						for (int c = 0; c < 4; c++)
							imageData[i + c] = 0;
					}
					for (vector<color3d>::iterator c = transparentColors.begin(); c != transparentColors.end(); c++) {
						if (abs(imageData[i + 0] - (*c).c.r * 255) <= 1 &&
							abs(imageData[i + 1] - (*c).c.g * 255) <= 1 &&
							abs(imageData[i + 2] - (*c).c.b * 255) <= 1) {
							for (int c = 0; c < 4; c++)
								imageData[i + c] = 0;
						}
					}
				}
			}
		}
	}
	
	//Unbind the image (to prevent any errors)
	ilBindImage(0);
	
	//Delete the image if anything went wrong
	if (!success) {
		ilDeleteImage(tempImage);
		tempImage = 0;
	}
	
	isLoaded = true;
}

void Texture::finalize()
{
	//OSSObjectLog << "Finalizing '" << name << "'..." << std::endl;
	
	//Only proceed if we have a valid image
	if (!tempImage)
		return;
	ilBindImage(tempImage);
	
	//Create the texture if required
	if (!textureID)
		glGenTextures(1, &textureID);
	
	//Actually create the OpenGL texture from the temporary bitmap
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB,
				 0,
				 GL_RGBA,
				 size.x,
				 size.y,
				 0,
				 ilGetInteger(IL_IMAGE_FORMAT),
				 GL_UNSIGNED_BYTE,
				 ilGetData());
	
	//Some texture parameters
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	//Get rid of the temporary image
	ilBindImage(0);
	/*ilDeleteImages(1, &tempImage);
	tempImage = 0;*/
	
	isFinalized = true;
}


void Texture::unfinalize()
{
	//Clear the OpenGL texture
	glDeleteTextures(1, &textureID);
	textureID = 0;
	
	isFinalized = false;
}

void Texture::unload()
{
	isLoaded = false;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Binding
//----------------------------------------------------------------------------------------------------

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, textureID);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, 0);
}
