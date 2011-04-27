#include "office.h"

using namespace OSS;
using namespace Classic;


ItemDescriptor OfficeItem::descriptor = {
	kOfficeType,
	kOfficeGroup,
	kFacilityCategory,
	1,
	(kNotBelowGroundAttribute),
	40000,
	int2(9, 1)
};





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

OfficeItem::OfficeItem(Tower * tower) : OccupiableItem(tower, &descriptor)
{
	setVariant(randui(0, 5));
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Workers
//----------------------------------------------------------------------------------------------------

bool OfficeItem::isAnyoneHere()
{
	return !people.empty();
}

void OfficeItem::didChangeOccupancy()
{
	//Transfer funds based on the vacancy.
	tower->funds->transfer(isOccupied() ? 10000 : -10000);	
	
	//If the office has just been moved into, initialize the missing workers.
	if (isOccupied()) {
		const Person::Type types[] = {
			Person::kSalesmanType,
			Person::kSalesmanType,
			Person::kManType,
			Person::kManType,
			Person::kWomanAType,
			Person::kWomanBType
		};
		
		while (workers.size() < 6) {
			OfficeWorker * worker = new OfficeWorker(tower, this);
			worker->setType(types[workers.size()]);
			workers.insert(worker);
		}
	}
	
	//Otherwise remove the workers.
	else {
		workers.clear();
	}
}

void OfficeItem::didAddPerson(Person * person)
{
	FacilityItem::didAddPerson(person);
	updateBackgroundIfNeeded.setNeeded();
}

void OfficeItem::didRemovePerson(Person * person)
{
	FacilityItem::didRemovePerson(person);
	updateBackgroundIfNeeded.setNeeded();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

bool OfficeItem::shouldOccupy()
{
	return tower->time->isBetween(7, 13) && tower->time->isWeekday();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

/*void OfficeItem::updateItem()
{
}*/

void OfficeItem::updateBackground()
{
	OccupiableItem::updateBackground();
	
	unsigned int slice = getTextureSliceIndex();
	
	backgrounds[0]->texture = Texture::named(getTextureBaseName());
	if (isOccupied())
		backgrounds[0]->textureRect = rectd(slice * 0.25, 0, 0.25, 1);
	else
		backgrounds[0]->textureRect = rectd(slice * 0.5, 0, 0.5, 1);
}

void OfficeItem::updateOccupyAt()
{
	setOccupyAt(tower->time->getTodayRandom(7, std::max<double>(7, tower->time->getTimeOfDay() + 0.5)));
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

string OfficeItem::getTextureBaseName()
{
	stringstream s;
	s << "simtower/facilities/office/";
	if (isOccupied())
		s << "inhabited/" << (getVariant() / 2);
	else
		s << "vacant";
	return s.str();
}

unsigned int OfficeItem::getTextureSliceIndex()
{
	unsigned int index = 0;
	
	if (isOccupied())
		index = (getVariant() % 2) * 2;
	else
		index = 0;
	
	if ((!isLit() && !isAnyoneHere()) || tower->time->isWeekend())
		index++;
	
	return index;
}

