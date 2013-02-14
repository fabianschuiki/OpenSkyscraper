/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include <string>

namespace OT {
	/**
	 * @brief Object encapsulating a file system path.
	 *
	 * A Path object introduces an abstraction layer to interact with file
	 * system paths across multiple platforms. Functions for ascending and de-
	 * scending the path hierarchy are provided.
	 */
	class Path
	{
	public:
		/// The path separator for the current operating system.
		static const char SEPARATOR;
		
		Path();
		Path(const char * path);
		Path(const std::string & path);
		
		Path up(int levels = 1) const;
		Path down(const std::string & comp) const;
		/// Convenience wrapper around down().
		Path operator + (const std::string & comp) const { return down(comp); }
		
		Path & remove(int levels = 1);
		Path & append(const std::string & comp);
		/// Convenience wrapper around append().
		Path & operator += (const std::string & comp) { return append(comp); }
		
		std::string name() const;
		
		std::string str() const;
		/// Convenience wrapper around str().
		inline operator std::string  () const { return path; }

		/// Returns a C string representation of the path.
		const char * c_str() const { return path.c_str(); }
		/// Convenience wrapper around c_str().
		inline operator const char * () const { return c_str(); }
		
		bool operator == (const Path & p) const { return path == p.path; }
		bool operator != (const Path & p) const { return path != p.path; }
		bool operator > (const Path & p) const { return path > p.path; }
		bool operator >= (const Path & p) const { return path >= p.path; }
		bool operator < (const Path & p) const { return path < p.path; }
		bool operator <= (const Path & p) const { return path <= p.path; }
	
	private:
		std::string path;
		
		void fixup();
		static void rtrim(std::string & s);
	};
}
