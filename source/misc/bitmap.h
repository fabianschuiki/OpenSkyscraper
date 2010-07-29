#ifndef OSS_BITMAP_H
#define OSS_BITMAP_H

#include "../general.h"
#include "../base/base.h"


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
