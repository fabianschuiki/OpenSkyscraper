#pragma once
#include "Path.h"

namespace OT
{
	class WindowsNEExecutable
	{
	public:
		struct Resource {
			int type;
			int id;
			int offset;
			int length;
			char * data;
			Resource() { data = NULL; }
			~Resource() { if (data) delete data; }
		};
		typedef std::map<int, Resource> Resources;
		typedef std::map<int, Resources> ResourceTable;
		
		ResourceTable resources;
		
		bool load(Path path);
		void dump(Path path);
	};
}
