/*
 *  bitmap.cpp
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 25.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#include "bitmap.h"

using namespace OSS;

Bitmap::Bitmap(std::string path) : Object()
{
	loadFile(path);
}

bool Bitmap::loadFile(std::string path)
{
	//Find the suffix of the path
	size_t suffixSeparator = path.find_last_of('.');
	if (suffixSeparator == std::string::npos) {
		std::cerr << description() << " " << __FUNCTION__ << ": unable to find the suffix in '" << path << "'" << std::endl;
		return false;
	}
	
	//Extract the suffix
	std::string suffix = path.substr(suffixSeparator + 1);
	std::transform(suffix.begin(), suffix.end(), suffix.begin(), std::ptr_fun<int, int>(std::tolower));
	
	//Call the appropriate loading function
	if (suffix.compare("png") == 0) return loadPNGFile(path);
	
	//Epic fail, print some helpful message and return false
	std::cerr << description() << " " << __FUNCTION__ << ": unknown suffix '" << suffix << "'" << std::endl;
	return false;
}
