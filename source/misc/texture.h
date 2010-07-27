/*
 *  texture.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 25.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef __OSS_TEXTURE_H__
#define __OSS_TEXTURE_H__
#include "opengl.h"
#include "object.h"
#include "bitmap.h"

namespace OSS {
	class Texture : public Object {
	public:
		GLuint texture;
		
		//Constructor and destructor which create and release the OpenGL texture
		Texture();
		~Texture();
		
		//Convenience initializers which immediately call the appropriate loading function
		Texture(Bitmap * bitmap);
		Texture(std::string path);
		
		//Designated loading function
		void loadBitmap(Bitmap * bitmap);
		
		//Convenience loading function
		void loadFile(std::string path);
	};
}

#endif
