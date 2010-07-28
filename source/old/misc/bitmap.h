/*
 *  bitmap.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 25.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef __OSS_BITMAP_H__
#define __OSS_BITMAP_H__
#include "common.h"
#include "object.h"

namespace OSS {
	class Bitmap : public Object {
	public:
		int2 size;
		void * data;
		
		//Convenience constructors which immediately calls loadFile
		Bitmap(std::string path);
		
		//Generic loading function which guesses the file type from the path's suffix
		bool loadFile(std::string path);
		
		//Format-specific loading functions
		bool loadPNGFile(std::string path);
	};
}

#endif
