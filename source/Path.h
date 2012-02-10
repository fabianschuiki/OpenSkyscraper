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
		Path operator + (const std::string & comp) const { return down(comp); }
		
		Path & remove(int levels = 1);
		Path & append(const std::string & comp);
		Path & operator += (const std::string & comp) { return append(comp); }
		
		std::string name() const;
		
		std::string str() const;
		inline operator std::string  () const { return str(); }

		const char * c_str() const { return str().c_str(); }
		inline operator const char * () const { return c_str(); }
	
	private:
		std::string path;
		
		void fixup();
		static void rtrim(std::string & s);
	};
}
