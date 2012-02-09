#include <iostream>

#include "Path.h"

using namespace OT;
using std::string;

#ifdef __WINDOWS__
const char Path::SEPARATOR = '\\';
void dozefy(string & s)
{
	for (string::iterator c = s.begin(); c != s.end(); c++)
		if (*c == '/')
			*c = SEPARATOR;
}
#else
const char Path::SEPARATOR = '/';
#define dozefy(s) 
#endif


/** Initializes an empty path. */
Path::Path(){}

/** Initializes a path form the given C string. */
Path::Path(const char * path) : path(path) { dozefy(path); }

/** Initializes a path from the given C++ stirng. */
Path::Path(const string & path) : path(path) { dozefy(path); }


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
Path & Path::remove(int levels)
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
	return *this;
}

/** Appends the given path component. */
Path & Path::append(const string & comp)
{
	dozefy(comp);
	if (!path.empty() && *(path.end()-1) != SEPARATOR && !comp.empty() && *(comp.end()-1) != SEPARATOR)
		path += SEPARATOR;
	path += comp;
	return *this;
}

/** Returns the last path component. */
string Path::name() const
{
	size_t sep = path.find_last_of(SEPARATOR);
	if (sep == string::npos)
		sep = 0;
	else
		sep += 1;
	return path.substr(sep);
}

/** Returns a string representation of the path. */
string Path::str() const
{
	return path;
}
