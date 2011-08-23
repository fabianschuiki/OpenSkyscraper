#include <iostream>
#include "path.h"

//TODO: Add Windows here.
const char Path::SEPARATOR = '/';



Path::Path()
{
}

Path::Path(const char * path) : path(path)
{
}

Path::Path(const std::string & path) : path(path)
{
}



Path Path::up(int levels) const
{
	Path p(*this);
	p.remove(levels);
	return p;
}

Path Path::down(const std::string & comp) const
{
	Path p(*this);
	p.append(comp);
	return p;
}



void Path::remove(int levels)
{
	//Try to cut off as many levels as possible.
	if (!path.empty()) {
		
		//Get the end of the string.
		std::string::iterator it = (path.end() - 1);
		
		//Move backward the given amount of levels.
		bool anyLetters = false;
		while (it != path.begin() && levels > 0) {
			if (*it == SEPARATOR) {
				if (anyLetters) {
					anyLetters = false;
					levels--;
				}
			} else {
				anyLetters = true;
			}
			if (levels > 0)
				it--;
		}
		
		//Cut off stuff.
		path.erase(it, path.end());
	}
}

void Path::append(const std::string & comp)
{
	if (!path.empty() && *(path.end() - 1) != SEPARATOR)
		path += SEPARATOR;
	path += comp;
}



std::string Path::description() const
{
	return path;
}
