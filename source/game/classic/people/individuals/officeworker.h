#ifndef OSS_CLASSIC_PEOPLE_INDIVIDUALS_OFFICEWORKER_H
#define OSS_CLASSIC_PEOPLE_INDIVIDUALS_OFFICEWORKER_H

#include "../person.h"

#include "../../items/facilities/office/office.h"


namespace OSS {
	namespace Classic {
		class OfficeWorker : public Person {
			
			/**
			 * Initialization
			 */
		public:
			const Pointer<OfficeItem> office;
			
			OfficeWorker(Tower * tower, OfficeItem * office);
			virtual string getTypeName() const { return "office/worker"; }
		};
	}
}


#endif

