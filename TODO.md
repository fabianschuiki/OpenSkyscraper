OpenSkyscraper TODO
===================


Tower
-----

	- Add some methods to find the closest transport item.


	### Decoration

	- Create a `TowerDecoration` class similar to `TowerBackground`, `TowerTime`, etc., which adds the little crane ontop of the tower. This crane is relocated if an item pushes the boundaries of the tower vertically. I.e., if you build the very first item of a floor, the crane gets positioned on the items roof on the far left.

	- Add fire ladders to the tower. The best idea would probably be to use the `getItems(int)` function together with the floor for which you are updating the fire ladders. Create the union rect over all items on that floor and position the ladders on the left and right of that rect.
	
	- Move the drawing of the entrances of the tower from the lobby class to the tower decoration.
	
	
	###ÊStructure
	
	- At the moment it is possible to build two unconnected items on a floor. This was not the case in SimTower, so we should modify the `constructItem()` function to check whether there already is a cell on that floor. If there is, the the distance between it and the construction rect needs to be filled with floor items. Note that `getCell(int2)` will return
	a cell if there is at least a floor cell at this location. So DON'T use `getItems(int)` to find any pre-existing stuff on that floor; use `getCells(int)` instead.


Items
-----

	- When two adjacent items are collapsed, they both show the construction animation. It should be possible to only show the animation for the part that was actually added to cause the collapse.
	- Create classes for all items of SimTower.
	- Implement transport facility's capability to move people.
	- Add people to the office item and animate them appropriately.
	- Add people to all other items.
	
	
	### Items to be created
	
	- Fast Food, `FastFoodItem`, `facilities/entertainment/fastfood.h|.cpp`
	- Shop, `ShopItem`, `facilities/entertainment/shop.h|.cpp`
	- Cinema, `CinemaItem`, `facilities/entertainment/cinema.h|.cpp`
	- Party Hall, `PartyHallItem`, `facilities/entertainment/partyhall.h|.cpp`
	- Parking Ramp, `ParkingRampItem`, `facilities/infrastructure/parkingramp.h|.cpp`
	- Parking Space, `ParkingSpaceItem`, `facilities/infrastructure/parkingspace.h|.cpp`
	- Recycling Center, `RecyclingCenterItem`, `facilities/infrastructure/recyclingcenter.h|.cpp`
	- Metro Station, `MetroStationItem`, `facilities/infrastructure/metrostation.h|.cpp`
	- Cathedral, `CathedralItem`, `facilities/services/cathedral.h|.cpp`
	- Security, `SecurityItem`, `facilities/services/security.h|.cpp`
	- Medical Center, `MedicalCenterItem`, `facilities/services/medicalcenter.h|.cpp`
	- Condo, `CondoItem`, `facilities/condo/condo.h|.cpp`


User Interface
--------------	
	
	### Tools Window
	
	- **ASAP:** Create a ToolsWindow class which contains UI for the different tower modification tools (like bulldozer, finger, lobby, office, etc.). Have a look at the `ControlWindow` class for how to do that.
	
	- It's probably necessary to create a new `GUI::Button` subclass of `GUI::View` which implements a button. I.e. accepts multiple textures and texture rects for different states (normal, pressed, disabled, etc.) and responds to user interaction properly.
	
	- In case of the construction tools (Lobby, etc.), I'd suggest to create a `ConstructionGroupButton` subclass of `GUI::Button` which maintains a popup subview and a button for each item type in that group. The button should choose borrow its textures from the construction button currently selected in its group.
	
	
	### Control Window
	
	- Add funds and population values
	- Add weekday, date, quarter and year information
	
	
	### Map Window
	
	The map window should show a thumbnail of the tower and some buttons to activate overlay. This has **low priority** until the inner workings of the simulation are done.


GUI
---
	
	Maybe we should use CEGUI?
