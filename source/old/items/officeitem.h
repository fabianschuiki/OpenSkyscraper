/*
 *  officeitem.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 26.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef __OSS_OFFICEITEM_H__
#define __OSS_OFFICEITEM_H__
#include "item.h"

namespace OSS {
	class OfficeItem : public Item {
	public:
		
		OfficeItem() : Item() {
			unitSize.x = 9;
			minSize = unitSize;
			frame.size = minSize;
		}
		
		virtual const GLfloat * debugGLColor() { static const GLfloat c[3] = {0, 0, 1}; return c; }
	};
}

#endif
