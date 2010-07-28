/*
 *  bitmap.cpp
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 25.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#include "bitmap.h"
#include <png.h>

using namespace OSS;

bool Bitmap::loadPNGFile(std::string path)
{
	OSSFuncLog();
	
	//Open file as binary
	FILE *fp = fopen(path.c_str(), "rb");
	if (!fp)
	{
		OSSObjectError << "unable to fopen " << path << std::endl;
		return false;
	}
	
	//Create PNG struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		fclose(fp);
		OSSObjectError << "unable to png_create_read_struct" << std::endl;
		return false;
	}
	
	//create png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
		fclose(fp);
		OSSObjectError << "unable to png_create_info_struct for info" << std::endl;
		return false;
	}
	
	//create png info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info) {
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		fclose(fp);
		OSSObjectError << "unable to png_create_info_struct for end info" << std::endl;
		return false;
	}
	
	//png error stuff, not sure libpng man suggests this.
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		OSSObjectError << "some error occured inside libpng" << std::endl;
		return false;
	}
	
	//init png reading
	png_init_io(png_ptr, fp);
	
	//let libpng know you already read the first 8 bytes
	//png_set_sig_bytes(png_ptr, 8);
	
	// read all the info up to the image data
	png_read_info(png_ptr, info_ptr);
	
	//variables to pass to get info
	int bit_depth, color_type;
	png_uint_32 twidth, theight;
	
	// get info about png
	png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type,
				 NULL, NULL, NULL);
	
	//update width and height based on png info
	size.x = twidth;
	size.y = theight;
	OSSObjectLog << "loading PNG of size " << size.description() << std::endl;
	
	// Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);
	
	// Row size in bytes.
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	
	// Allocate the image_data as a big block, to be given to opengl
	png_byte *image_data = new png_byte[rowbytes * theight];
	if (!image_data) {
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return false;
	}
	
	//row_pointers is for pointing to image_data for reading the png with libpng
	png_bytep *row_pointers = new png_bytep[theight];
	if (!row_pointers) {
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		delete[] image_data;
		fclose(fp);
		return false;
	}
	// set the individual row_pointers to point at the correct offsets of image_data
	for (int i = 0; i < theight; ++i)
		row_pointers[theight - 1 - i] = image_data + i * rowbytes;
	
	//read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);
	
	data = image_data;
	
	//clean up memory and close stuff
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	//delete[] image_data;
	delete[] row_pointers;
	fclose(fp);
	
	return true;
}
