/*
 *  texture.cpp
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 25.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#include "texture.h"

using namespace OSS;

Texture::Texture() : Object()
{
	//Initialize the OpenGL texture
	glGenTextures(1, &texture);
}

Texture::~Texture()
{
	//Release the OpenGL texture
	if (texture) {
		glDeleteTextures(1, &texture);
		texture = 0;
	}
}

Texture::Texture(Bitmap * bitmap)
{
	Texture();
	loadBitmap(bitmap);
}
Texture::Texture(std::string path)
{
	Texture();
	loadFile(path);
}

void Texture::loadBitmap(Bitmap * bitmap)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap->size.x, bitmap->size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::loadFile(std::string path)
{
	OSSObjectLog << "loading texture '" << path << "'" << std::endl;
	
	//Initialize a new bitmap with the contents of the image at the given path
	Bitmap bitmap(path);
	
	//Load the texture from this bitmap
	loadBitmap(&bitmap);
}
