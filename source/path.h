#pragma once
#include <string>


class Path {
public:
	/** Initializes an empty path. */
	Path();
	/** Initializes a path form the given C string. */
	Path(const char * path);
	/** Initializes a path from the given C++ stirng. */
	Path(const std::string & path);
	
	/** Returns a path instance which is the given amount of levels higher in the path hierarchy
	 than the path that received the call. */
	Path up(int levels = 1) const;
	/** Returns a path with the given path component appended. */
	Path down(const std::string & comp) const;
	
	/** Climbs up the path hierarchy by the given amount of levels. */
	void remove(int levels = 1);
	/** Appends the given path component. */
	void append(const std::string & comp);
	
	/** Returns a string representation of the path. */
	std::string description() const;
	inline operator std::string () const { return description(); }
	
	/** The path separator for the target OS. */
	static const char SEPARATOR;
	
private:
	std::string path;
};
