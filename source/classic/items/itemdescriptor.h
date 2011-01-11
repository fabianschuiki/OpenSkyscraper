#ifndef OSS_CLASSIC_ITEMS_ITEMDESCRIPTOR_H
#define OSS_CLASSIC_ITEMS_ITEMDESCRIPTOR_H

#include "../external.h"


namespace OSS {
	namespace Classic {
		
		//Item Types
		typedef enum {
			kNoneType = 0,
			
			//Structure
			kLobbyType,
			kFloorType,
			kStairsType,
			kEscalatorType,
			
			//Elevator
			kStandardElevatorType,
			kServiceElevatorType,
			kExpressElevatorType,
			
			//Office
			kOfficeType,
			
			//Hotel
			kSingleRoomType,
			kDoubleRoomType,
			kSuiteType,
			
			//Entertainment
			kFastFoodType,
			kRestaurantType,
			kShopType,
			kCinemaType,
			kPartyHall,
			
			//Infrastructure
			kParkingRampType,
			kParkingSpaceType,
			kRecyclingCenterType,
			kMetroType,
			
			//Services
			kCathedrakType,
			kSecurityType,
			kMedicalCenterType,
			kHousekeepingType,
			
			//Condo
			kCondoType,
			
			kMaxType
		} ItemType;
		
		//Item Groups
		typedef enum {
			kNoneGroup = 0,
			
			kStructureGroup,
			kElevatorGroup,
			kOfficeGroup,
			kHotelGroup,
			kEntertainmentGroup,
			kInfrastructureGroup,
			kServicesGroup,
			
			kMaxGroup
		} ItemGroup;
		
		//Item Categories
		typedef enum {
			kFacilityCategory,
			kTransportCategory
		} ItemCategory;
		
		//Item Attributes
		typedef enum {
			kFlexibleWidthAttribute		= (1 << 0),
			kEvery15thFloorAttribute	= (1 << 1),
			kNotAboveGroundAttribute	= (1 << 2),
			kNotBelowGroundAttribute	= (1 << 3),
			kAllowedOnGroundAttribute	= (1 << 4),
			kUndestructibleAttribute	= (1 << 5)
		} ItemAttributes;
		
		//Item Descriptor
		typedef struct {
			ItemType type;
			ItemGroup group;
			ItemCategory category;
			unsigned short minRating;
			unsigned short attributes;
			unsigned int price;
			int2 cells;
			int2 minUnit;
			rectmaski mask;
		} ItemDescriptor;
	}
}


#endif
