#pragma once
#include <string>

namespace OT
{
	class Path
	{
	public:
		static const char SEPARATOR;
	
		Path();
		Path(const char * path);
		Path(const std::string & path);
	
		Path up(int levels = 1) const;
		Path down(const std::string & comp) const;
	
		void remove(int levels = 1);
		void append(const std::string & comp);
		
		std::string name() const;
		
		std::string str() const;
		inline operator std::string  () const { return str(); }

		const char * c_str() const { return str().c_str(); }
		inline operator const char * () const { return c_str(); }
	
	private:
		std::string path;
	};
}
