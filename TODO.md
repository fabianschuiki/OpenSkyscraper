General
-------

- `mapWindow` should be its own class `MapWindow`, like the other two.
- Cache the result of the KWAJ decompression so the game doesn't decompress SIMTOWER.EX_ everytime it is relaunched.


### SFML 2
Maybe we should move everything to SFML version 2.0.

### Pausing doesn't affect elevators
When pausing the game, elevators keep moving as if the game was unpaused. The weird thing is that the elevators react to speedup by moving faster. Why wouldn't they react to the speeddown?

### Clean up CMakeLists.txt
There's a lot of old stuff in the CMakeLists.txt file, such as the Lua and ObjectiveLua stuff, as well as CEGUI and the like. Clean this up so the compiling process won't break on stuff that's not even required anymore ;)

### Game Speed
The game speed seems to be rather fast compared to elevator movements. Cinema customers are hardly able to arrive at the theatre in time. Maybe we should slow down the time a bit? Or speed up the elevators? I don't remember how fast the regular elevators actually were.

### Decorations
Add tower decorations:

- [DONE] Fire Stairs
- [DONE] Crane at the top of the tower

### Construction Animation
Animate the construction of items. The `construction/*` bitmaps should help with that. The `solid` bitmap is for regular items, the `grid` bitmap for lobbies, car parkings, etc.

### Different Access Floors for Items
At the moment, people enter all items on floor 0 (relative to the item). Certain items, such as the Metro station, are accessed via floor 1. The item prototype should contain a field that enables a relative shift of the access floor.


Background Noise
----------------
[DONE] Many items produce background noise in the original game. E.g. Fast Foods, Offices and Condos had really distinctive sounds. Write a system that regularly iterates over a subset of all visible items and asks each for a background sound to be played. The system should be fair, i.e. each item should have a fair chance of playing back some sound.

[DONE] My current implementation idea: Iterate through the visible items at regular intervals, say 0.5s. Each item has a playback chance calculated as `p = itemArea / screenArea = itemWidth * itemHeight / screenArea`. Through a random number an item is picked that is asked for a sound which is then played back. One caveat here is that simply using `screenArea` in the above formula yields a total probability of `p >= 1`, since the total area of all items may actually be larger than the screen area due to items being only partially visible. Substitute `screenArea` in the above formula with `K = max(screenArea, summedItemAreas)`.


Item::FastFood
--------------
[DONE] In order to make the customers arrive at the tower, the fast food item has to iterate over all its
customers at each frame. This could be made more efficient if there was an arrival queue which,
after today's customers are initialized, is populated with the customers ordered by their arrival
time. The fast food would then only have to check if the queue's frontmost customer has
`c->arrivalTime >= game->time.hours` , pop the customer, and in case
`game->time.checkHour(c->arrivalTime)` make the customer arrive.

- New customers arrive until the fast food closes, or at least the transit times make it seem so. Maybe we should change their behaviour such that customers won't enter the tower after 1900. This would also make the fast foods look empty towards the end of the day, which is a nice thing to have.

- Store the customers to disk. At the moment, when you save the debug tower using F2 and reload the game, you end up with open yet empty fast foods.


Person
------
Handle change of routes (from accessible to inaccessible and vice-versa) for 
people already travelling. Need to stop them from continuing their journey 
to prevent crashes when transport routes are changed, and transit appropriately 
out of the flow of traffic.


Hotels
------
Implement the hotel system. For this, maybe have a look at the implementation in the `master` branch. It is not very well-written but reproduces the original game's behaviour. In general what should happen is:

- Hotels should start to get populated after 17:00. Occupants immediately queue up in the lobby.
- Occupants should move out for dinner after ~0.5h after entering their room. This means they will look for Item::Restaurant items, or leave the tower.
- All occupants should go to sleep between 23:00 and 1:30.
- Occupants should get up between 6:00 and 8:00 and leave between 8:00 and 10:00 and leave the room back in the dirty state.
- Housekeepers should move to dirty bedrooms, stay there and clean for a fixed amount of absolute time. This is important since otherwise the housekeepers would take far longer to clean a room during noon than at 10:00. After leaving the hotel room, the room should be reset to the tidy state.


Item::PartyHall
---------------

- Check the original game for the exact timing of parties. I think I recall that there were two parties held each day, but I don't remember when.
- Check how much money you get back from party halls. Maybe that's based on the number of people visiting?
- Make people arrive/leave.


Item::Cinema
------------

- Check playing times in the original game.
- Implement income simulation. It is somehow based on the number of patrons I guess.
- Did the original game have a break halfway through the movie?


Item::Elevator
--------------

- [DONE] Add animation of people leaving an elevator car. At the moment, only the "step in" animation is shown when people enter the elevator.


Item::Cinema
------------

- Should be able to be built underground (if I recall correctly).


Item::Metro
-----------

- Make trains arrive/leave in regular absolute time intervals (see Time object's absolute time member).
- Metro stations should create customers that visit shops below ground. Each train should bring new customers, and haul away passengers waiting on the platform.
- Add metro tracks. This should probably go into the same category as the fire stairs and crane, i.e. tower decorations.