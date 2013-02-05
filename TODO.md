General
-------

- create `CommmandLineArguments` class which handles interpretation of the command line arguments
  (e.g. `--dump-simtower <path>`)
- create `FileSystem` class to do file manipulation (move, copy, mkdir, etc.)
	- adjust `WindowsNEExecutable::dump`
- In `Route`, keep track of how many stairs and elevators were used. Then make the pathfinder limit
  the amount of stairs and elevators that are used per route. Otherwise algorithm complexity will
  explode.
- Modify `Time` so that `absolute` is a day counter that advances evenly. Add some code that
  calculates the current hour by scaling the absolute time differently for different phases of the
  day to create the effect of time running at different speeds.
- `mapWindow` should be its own class `MapWindow`, like the other two.


### SFML 2
Maybe we should move everything to SFML version 2.0.

### Audio
Put a limitation mechanim in place that prevents the same sound from being played at the same time. One way of doing this would be to create a map that contains the name and timestamp for each sound played. Whenever a new sound is to be played, the system checks whether enough time expired since the timestamp for this sound name, thus preventing sounds from playing 10-fold.

### Pausing doesn't affect elevators
When pausing the game, elevators keep moving as if the game was unpaused. The weird thing is that the elevators react to speedup by moving faster. Why wouldn't they react to the speeddown?

### Clean up CMakeLists.txt
There's a lot of old stuff in the CMakeLists.txt file, such as the Lua and ObjectiveLua stuff, as well as CEGUI and the like. Clean this up so the compiling process won't break on stuff that's not even required anymore ;)


Background Noise
----------------
Many items produce background noise in the original game. E.g. Fast Foods, Offices and Condos had really distinctive sounds. Write a system that regularly iterates over a subset of all visible items and asks each for a background sound to be played. The system should be fair, i.e. each item should have a fair chance of playing back some sound.

My current implementation idea: Iterate through the visible items at regular intervals, say 0.5s. Each item has a playback chance calculated as `p = itemArea / screenArea = itemWidth * itemHeight / screenArea`. Through a random number an item is picked that is asked for a sound which is then played back. One caveat here is that simply using `screenArea` in the above formula yields a total probability of `p >= 1`, since the total area of all items may actually be larger than the screen area due to items being only partially visible. Substitute `screenArea` in the above formula with `K = max(screenArea, summedItemAreas)`.


Item::FastFood
--------------
[DONE] In order to make the customers arrive at the tower, the fast food item has to iterate over all its
customers at each frame. This could be made more efficient if there was an arrival queue which,
after today's customers are initialized, is populated with the customers ordered by their arrival
time. The fast food would then only have to check if the queue's frontmost customer has
`c->arrivalTime >= game->time.hours` , pop the customer, and in case
`game->time.checkHour(c->arrivalTime)` make the customer arrive.

New customers arrive until the fast food closes, or at least the transit times make it seem so. Maybe we should change their behaviour such that customers won't enter the tower after 1900. This would also make the fast foods look empty towards the end of the day, which is a nice thing to have.


Person
------
Handle change of routes (from accessible to inaccessible and vice-versa) for 
people already travelling. Need to stop them from continuing their journey 
to prevent crashes when transport routes are changed, and transit appropriately 
out of the flow of traffic.
