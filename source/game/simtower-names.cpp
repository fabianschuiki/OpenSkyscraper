#include "simtower.h"

using namespace OSS;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Bitmaps
//----------------------------------------------------------------------------------------------------

SimTower::ResourceName SimTower::bitmapResourceNames[] = {
	/**
	 * User Interface
	 */
	
	//Toolbox
	// -> play/pause
	{0x25A, "ui/toolbox/pause/normal"},				//pause, normal
	{0x25B, "ui/toolbox/pause/pressed"},			//pause, pressed
	{0x258, "ui/toolbox/play/normal"},				//play, normal
	{0x259, "ui/toolbox/play/pressed"},				//play. pressed
	// -> tools
	{0x25C, "ui/toolbox/tools/normal"},				//normal
	{0x25D, "ui/toolbox/tools/selected"},			//selected
	{0x25E, "ui/toolbox/tools/disabled"},			//disabled
	// -> construction
	{0x12C, "ui/toolbox/construction/normal"},		//normal
	{0x12D, "ui/toolbox/construction/selected"},	//selected
	{0x12E, "ui/toolbox/construction/disabled"},	//disabled
	
	//Time
	// -> background
	{0x140, "ui/control/background"},
	// -> widgets
	{0x141, "ui/control/watch"},
	{0x143, "ui/control/star/empty"},
	{0x142, "ui/control/star/filled"},
	{0x147, "ui/control/towerrating"},
	
	//Map
	// -> background
	{0x160, "ui/map/background"},
	// -> selector
	{0x136, "ui/map/selector/normal"},	//normal selector buttons (eval, pricing, etc.)
	{0x137, "ui/map/selector/pressed"},	//pressed selector buttons
	{0x138, "ui/map/selector/normal/hoteldisabled"},	//normal selector buttons with hotel disabled
	// -> overlay key
	{0x139, "ui/map/key/eval"},		//key to the eval overlay
	{0x13A, "ui/map/key/pricing"},	//key to the pricing overlay
	{0x13B, "ui/map/key/hotel"},	//key to the hotel overlay
	
	//Images
	{0x2710, "ui/images/terrorist"},
	{0x2711, "ui/images/firechopper"},
	{0x2712, "ui/images/vip"},
	{0x2713, "ui/images/treasure"},
	{0x2714, "ui/images/fire0"},
	{0x2715, "ui/images/fire1"},
	{0x2716, "ui/images/star"},
	
	
	
	
	/**
	 * Background
	 */
	
	//Ground
	{0x351, "background/ground"},
	
	//Sky
	{0x352, "background/sky/0"},
	{0x353, "background/sky/1"},
	{0x354, "background/sky/2"},
	{0x355, "background/sky/3"},
	{0x356, "background/sky/4"},
	{0x357, "background/sky/5"},
	{0x358, "background/sky/6"},
	{0x359, "background/sky/7"},
	{0x35A, "background/sky/8"},
	{0x35B, "background/sky/9"},
	
	//Clouds
	{0x384, "background/cloud/0"},
	{0x385, "background/cloud/1"},
	{0x386, "background/cloud/2"},
	{0x387, "background/cloud/3"},
	
	//City
	{0x389, "background/city"},
	
	
	
	/**
	 * Decoration
	 */
	
	//Tower
	{0x3E9, "decoration/entrance"},	//tower entrances (left and right)
	{0x3EA, "decoration/crane"},	//crane that sits ontop of the tower
	{0x42D, "decoration/stairs"},	//fire escape stairs
	
	//Santa
	{0x388, "decoration/santa"},
	
	
	
	/**
	 * Facilities and Transport
	 */
	
	//Structure
	// -> lobby
	{0x1388, "facilities/lobby/ceiling"},
	// -> stairs
	{0x9A8, "transport/stairs/lower/0"},
	{0x9A9, "transport/stairs/lower/1"},
	{0x968, "transport/stairs/upper/0"},
	{0x969, "transport/stairs/upper/1"},
	// -> escalator
	{0xAE8, "transport/escalator/lower"},
	{0xAA8, "transport/escalator/upper"},
	
	
	//Elevator
	// -> normal digits
	{0x7E8, "transport/floordigits/one/normal"},	//one digit floors
	{0x7EA, "transport/floordigits/two/ms/normal"},	//two digit floors, most significant digit
	{0x7E9, "transport/floordigits/two/ls/normal"},	//two digit floors, least significant digit
	// -> highlighted digits
	{0x7EB, "transport/floordigits/one/highlighted"},		//one digit floors
	{0x7ED, "transport/floordigits/two/ms/highlighted"},	//two digit floors, most significant digit
	{0x7EC, "transport/floordigits/two/ls/highlighted"},	//two digit floors, least significant digit
	// -> standard elevator
	{0x428, "transport/elevator/standard/empty"},
	{0x429, "transport/elevator/standard/occupied"},
	// -> service elevator
	{0x42A, "transport/elevator/service"},
	// -> express elevator
	{0x42B, "transport/elevator/express"},
	
	
	//Office
	{0x5A8, "facilities/office/inhabited0"},	//office types 0 and 1
	{0x5A9, "facilities/office/inhabited1"},	//office types 2 and 3
	{0x5AA, "facilities/office/inhabited2"},	//office types 4 and 5
	{0x5AB, "facilities/office/vacant"},		//vacant office
	{0x5E8, "facilities/office/workers"},		//office workers overlay
	
	
	//Hotel
	{0x5ED, "facilities/hotel/guests"},			//guests overlay
	{0x5EE, "facilities/hotel/housekeeper"},	//housekeeper overlay
	
	// -> single room
	//  0
	{0x4A8, "facilities/hotel/single/0/0"},		//single room type 0, day occupied state
	{0x4A9, "facilities/hotel/single/0/1"},		//single room type 0, other states
	//  1
	{0x4AA, "facilities/hotel/single/1/0"},		//single room type 1, day occupied state
	{0x4AB, "facilities/hotel/single/1/1"},		//single room type 1, other states
	
	// -> double room
	//  0
	{0x4E8, "facilities/hotel/double/0/0"},		//double room type 0, day occupied state
	{0x4E9, "facilities/hotel/double/0/1"},		//double room type 0, other states
	//  1
	{0x4EA, "facilities/hotel/double/1/0"},		//double room type 1, day occupied state
	{0x4EB, "facilities/hotel/double/1/1"},		//double room type 1, other states
	//  2
	{0x4EC, "facilities/hotel/double/2/0"},		//double room type 2, day occupied state
	{0x4ED, "facilities/hotel/double/2/1"},		//double room type 2, other states
	//  3
	{0x4EE, "facilities/hotel/double/3/0"},		//double room type 3, day occupied state
	{0x4EF, "facilities/hotel/double/3/1"},		//double room type 3, other states
	
	// -> suite
	//  0
	{0x528, "facilities/hotel/suite/0/0"},		//suite type 0, day occupied state
	{0x529, "facilities/hotel/suite/0/1"},		//suite type 0, other states
	//  1
	{0x52A, "facilities/hotel/suite/1/0"},		//suite type 1, day occupied state
	{0x52B, "facilities/hotel/suite/1/1"},		//suite type 1, other states
	
	
	//Condo
	{0x5E9, "facilities/condo/inhabitants"},		//inhabitants overlay
	//  0
	{0x628, "facilities/condo/0/day/inhabited"},	//condo type 0, day, inhabited
	{0x629, "facilities/condo/0/night/awake"},		//condo type 0, night, inhabitants awake
	{0x62A, "facilities/condo/0/night/asleep"},		//condo type 0, night, inhabitants asleep
	{0x62B, "facilities/condo/0/day/forsale"},		//condo type 0, empty, day
	{0x62C, "facilities/condo/0/night/forsale"},	//condo type 0, empty, night
	//  1
	{0x632, "facilities/condo/1/day/inhabited"},	//condo type 1, day, inhabited
	{0x633, "facilities/condo/1/night/awake"},		//condo type 1, night, inhabitants awake
	{0x634, "facilities/condo/1/night/asleep"},		//condo type 1, night, inhabitants asleep
	{0x635, "facilities/condo/1/day/forsale"},		//condo type 1, empty, day
	{0x636, "facilities/condo/1/night/forsale"},	//condo type 1, empty, night
	//  2
	{0x62D, "facilities/condo/2/day/inhabited"},	//condo type 2, day, inhabited
	{0x62E, "facilities/condo/2/night/awake"},		//condo type 2, night, inhabitants awake
	{0x62F, "facilities/condo/2/night/asleep"},		//condo type 2, night, inhabitants asleep
	{0x630, "facilities/condo/2/day/forsale"},		//condo type 2, empty, day
	{0x631, "facilities/condo/2/night/forsale"},	//condo type 2, empty, night
	
	
	//Entertainment
	// -> fast food
	//  0
	{0x6E8, "facilities/fastfood/0/0"},	//fast food type 0 empty and low patronage
	{0x6E9, "facilities/fastfood/0/1"},	//fast food type 0 high patronage and closed
	//  1
	{0x6EA, "facilities/fastfood/1/0"},	//fast food type 1 empty and low patronage
	{0x6EB, "facilities/fastfood/1/1"},	//fast food type 1 high patronage and closed
	//  2
	{0x6EC, "facilities/fastfood/2/0"},	//fast food type 2 empty and low patronage
	{0x6ED, "facilities/fastfood/2/1"},	//fast food type 2 high patronage and closed
	//  3
	{0x6EE, "facilities/fastfood/3/0"},	//fast food type 3 empty and low patronage
	{0x6EF, "facilities/fastfood/3/1"},	//fast food type 3 high patronage and closed
	//  4
	{0x6F0, "facilities/fastfood/4/0"},	//fast food type 4 empty and low patronage
	{0x6F1, "facilities/fastfood/4/1"},	//fast food type 4 high patronage and closed
	
	// -> restaurant
	{0x5EC, "facilities/restaurant/waiters"},
	//  0
	{0x568, "facilities/restaurant/0/0"},	//restaurant type 0, empty and low patronage
	{0x569, "facilities/restaurant/0/1"},	//restaurant type 0, high patronage and closed
	//  1
	{0x56A, "facilities/restaurant/1/0"},	//restaurant type 1, empty and low patronage
	{0x56B, "facilities/restaurant/1/1"},	//restaurant type 1, high patronage and closed
	//  2
	{0x56C, "facilities/restaurant/2/0"},	//restaurant type 2, empty and low patronage
	{0x56D, "facilities/restaurant/2/1"},	//restaurant type 2, high patronage and closed
	//  3
	{0x56E, "facilities/restaurant/3/0"},	//restaurant type 3, empty and low patronage
	{0x56F, "facilities/restaurant/3/1"},	//restaurant type 3, high patronage and closed
	//  4
	{0x570, "facilities/restaurant/4/0"},	//restaurant type 4, empty and low patronage
	{0x571, "facilities/restaurant/4/1"},	//restaurant type 4, high patronage and closed
	
	// -> shop
	{0x668, "facilities/shop/0"},
	{0x669, "facilities/shop/1"},
	{0x66A, "facilities/shop/2"},
	{0x66B, "facilities/shop/3"},
	{0x66C, "facilities/shop/4"},
	{0x66D, "facilities/shop/5"},
	{0x66E, "facilities/shop/6"},
	{0x66F, "facilities/shop/7"},
	{0x670, "facilities/shop/8"},
	{0x671, "facilities/shop/9"},
	{0x672, "facilities/shop/10"},
	{0x673, "facilities/shop/forrent"},
	{0x674, "facilities/shop/closed"},
	
	// -> cinema
	{0xCA8, "facilities/cinema/screen/0/lower"},
	{0xCA9, "facilities/cinema/screen/1/lower"},
	{0xC68, "facilities/cinema/screen/0/upper"},
	{0xC69, "facilities/cinema/screen/1/upper"},
	{0x8A8, "facilities/cinema/lower"},
	{0x868, "facilities/cinema/upper"},
	
	// -> party hall
	{0xB28, "facilities/partyhall/lower"},
	{0xB68, "facilities/partyhall/upper"},
	
	
	//Infrastructure
	// -> parking ramp
	{0xEE8, "facilities/parking/ramp/lower"},	//lowest ramp, right side closed
	{0xEE9, "facilities/parking/ramp/middle"},	//intermediate ramp, both sides open
	{0xEEA, "facilities/parking/ramp/upper"},	//upmost ramp, entrance on the left
	
	// -> parking space
	{0x6A8, "facilities/parking/space/empty"},		//empty parking space
	{0x6A9, "facilities/parking/space/occupied"},	//occupied and unreachable parking space
	
	// -> recycling center
	{0x8E8, "facilities/recycling/empty"},			//empty recycling center
	{0x929, "facilities/recycling/lower/filled/0"},	//lower floor, fill state 0
	{0x92A, "facilities/recycling/lower/filled/1"},	//lower floor, fill state 1
	{0x92B, "facilities/recycling/lower/filled/2"},	//lower floor, fill state 2
	{0x92C, "facilities/recycling/lower/filled/3"},	//lower floor, fill state 3
	{0x92D, "facilities/recycling/lower/filled/4"},	//lower floor, fill state 4
	{0x92E, "facilities/recycling/lower/truck"},	//lower floor, recycling truck leaving
	{0x928, "facilities/recycling/lower/empty"},	//lower floor, empty
	{0x8E9, "facilities/recycling/upper/filled/0"},	//upper floor, fill state 0
	{0x8EA, "facilities/recycling/upper/filled/1"},	//upper floor, fill state 1
	{0x8EB, "facilities/recycling/upper/filled/2"},	//upper floor, fill state 2
	{0x8EC, "facilities/recycling/upper/filled/3"},	//upper floor, fill state 3
	{0x8ED, "facilities/recycling/upper/filled/4"},	//upper floor, fill state 4
	
	// -> metro
	{0xC28, "facilities/metro/empty/floor0"},	//lowest floor with tracks
	{0xBE8,	"facilities/metro/empty/floor1"},	//middle floor with screen
	{0xBA8, "facilities/metro/empty/floor2"},	//upper floor with ticket booth
	{0xC29, "facilities/metro/train/floor0"},	//lowest floor with train
	{0xBE9,	"facilities/metro/train/floor1"},	//middle floor with passengers
	{0xBA9, "facilities/metro/train/floor2"},	//upper floor
	{0xF28, "facilities/metro/tunnel"},
	
	
	//Services
	// -> cathedral
	{0xDE8, "facilities/cathedral/floor0/normal"},			//normal cathedral ground floor
	{0xDE9, "facilities/cathedral/floor0/wedding"},			//cathedral with wedding in progress
	{0xDEA, "facilities/cathedral/floor0/preparations"},	//cathedral with guests
	{0xDA8, "facilities/cathedral/floor1/normal"},			//normal cathedral
	{0xDA9, "facilities/cathedral/floor1/wedding"},			//cathedral lit up during wedding
	{0xD68, "facilities/cathedral/floor2/normal"},			//normal cathedral
	{0xD69, "facilities/cathedral/floor2/wedding"},			//cathedral lit up during wedding
	{0xD28, "facilities/cathedral/floor3/normal"},			//normal cathedral
	{0xD29, "facilities/cathedral/floor3/tower"},			//cathedral with tower rating
	{0xCE8, "facilities/cathedral/floor4/normal"},			//normal cathedral
	{0xCE9, "facilities/cathedral/floor4/tower"},			//cathedral with tower rating
	{0x828, "facilities/cathedral/bride&groom"},
	
	// -> security
	{0x768, "facilities/security"},
	
	// -> medical care
	{0x728, "facilities/medicalcenter/patients"},
	{0x729, "facilities/medicalcenter/open"},
	{0x72A, "facilities/medicalcenter/closed"},
	
	// -> house keeping
	{0x7A8, "facilities/housekeeping"},
	
	
	
	/**
	 * Construction
	 */
	
	{0x5EA, "construction/workers"},		//construction workers overlay
	{0xE28, "construction/floor"},			//the floor construction, merely a grid
	{0xE29, "construction/facility"},		//the facility construction, opaque and hosts workers
	
	
	
	/**
	 * Uncategorized
	 *
	 * Various bitmaps that don't belong into a category yet.
	 */
	
	//People
	{0x2BC, "people/normal"},		//normal people (black)
	{0x2BD, "people/stressed"},		//stressed people (intense red)
	{0x2BE, "people/named"},		//named people (blue)
	{0x2BF, "people/vip"},			//VIPs (yellow)
	{0x468, "people/walking"},		//walking people (black)
	{0x469, "people/security"},		//walking security officers (black)
	
	//Splashes
	{0x80, "splash/maxis"},
	{0x100, "splash/simtower"},
	
	//Overlay
	{0x3EB, "overlay"}, //overlay colors for eval, pricing and hotel inspection
	
	//Burning Fire
	{0xF68, "fire/flames/0"},
	{0xF69, "fire/flames/1"},
	{0xF6A, "fire/flames/2"},
	{0xF6B, "fire/flames/3"},
	{0xF6C, "fire/flame"},
	{0xF6D, "fire/chopper"},
	{0xFA8, "fire/burnedfloor"},
	
	{0, ""}
};





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Sounds
//----------------------------------------------------------------------------------------------------

SimTower::ResourceName SimTower::soundResourceNames[] = {
	/**
	 * Construction
	 */
	
	{0x1B58, "construction/normal"},		//sound for placing facilities and transport
	{0x1B59, "construction/flexible"},		//sound for flexible stuff like lobbies and floors
	{0x1B5A, "construction/impossible"},	//annoying "click" :)
	{0x1B5B, "construction/bulldoze"},
	
	{0, ""}
};
