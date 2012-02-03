#include "path.h"
#include <iostream>

using namespace OT;
using std::string;

//TODO: Add Windows here.
const char Path::SEPARATOR = '/';


/** Initializes an empty path. */
Path::Path(){}

/** Initializes a path form the given C string. */
Path::Path(const char * path) : path(path) {}

/** Initializes a path from the given C++ stirng. */
Path::Path(const string & path) : path(path) {}


/** Returns a path with the given amount of levels cut off. */
Path Path::up(int levels) const
{
	Path p(*this);
	p.remove(levels);
	return p;
}

/** Returns a path with the given path component appended. */
Path Path::down(const string & comp) const
{
	Path p(*this);
	p.append(comp);
	return p;
}

/** Climbs up the path hierarchy by the given amount of levels. */
void Path::remove(int levels)
{
	//Try to cut off as many levels as possible.
	if (!path.empty()) {
		
		//Get the end of the string.
		string::iterator it = (path.end() - 1);
		
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

/** Appends the given path component. */
void Path::append(const string & comp)
{
	if (!path.empty() && *(path.end() - 1) != SEPARATOR)
		path += SEPARATOR;
	path += comp;
}

/** Returns a string representation of the path. */
string Path::str() const
{
	return path;
}
