/*
 *  object.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 23.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef __OSS_OBJECT_H__
#define __OSS_OBJECT_H__
#include "common.h"
#include "macros.h"

#define OSSObjectLog std::cout << this->description() << " " << __FUNCTION__ << ": "
#define OSSObjectError std::cout << this->description() << " " << __FUNCTION__ << ": *** "

namespace OSS {
	class Object {
	public:
		int retainCount;
		
		Object() {
			retainCount = 0;
		}
		
		void retain() {
			retainCount++;
		}
		
		void release() {
			assert(retainCount > 0);
			retainCount--;
			if (retainCount <= 0)
				delete this;
		}
		
		//Runtime type information
		virtual std::string className() { return typeid(*this).name(); }
		virtual std::string instanceName() {
			char str[64];
			sprintf(str, "%s %p", this->className().c_str(), this);
			return str;
		}
		
		//Object description
		virtual std::string description() {
			std::string res = "<";
			res += this->instanceName();
			res += ">";
			return res;
		}
	};
}

#endif
