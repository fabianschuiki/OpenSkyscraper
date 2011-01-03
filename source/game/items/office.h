#ifndef OSS_OFFICE_H
#define OSS_OFFICE_H

#include "../../general.h"
#include "../facilityitem.h"


namespace OSS {
	class OfficeItem : public FacilityItem {
	public:
		static Item::Descriptor descriptor;
		
		//Initialization
	public:
		OfficeItem(Tower * tower);
		
		//Type
	private:
		unsigned int type;
	public:
		unsigned int getType();
		void setType(const unsigned int type);
		
		//Vacancy
	private:
		bool vacant;
	public:
		bool isVacant();
		void setVacant(const bool vacant);
		
		//Basic Sprites
		void updateBackground();
		
		//Simulation
		void advance(double dt);
	};
}


#endif
