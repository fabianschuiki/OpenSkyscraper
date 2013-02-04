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


Game
----

- `mapWindow` should be its own class `MapWindow`, like the other two.

### SFML 2
Maybe we should move everything to SFML version 2.0.

### Audio
Put a limitation mechanim in place that prevents the same sound from being played at the same time. One way of doing this would be to create a map that contains the name and timestamp for each sound played. Whenever a new sound is to be played, the system checks whether enough time expired since the timestamp for this sound name, thus preventing sounds from playing 10-fold.

### Pausing doesn't affect elevators
When pausing the game, elevators keep moving as if the game was unpaused. The weird thing is that the elevators react to speedup by moving faster. Why wouldn't they react to the speeddown?

Item::FastFood
--------------
In order to make the customers arrive at the tower, the fast food item has to iterate over all its
customers at each frame. This could be made more efficient if there was an arrival queue which,
after today's customers are initialized, is populated with the customers ordered by their arrival
time. The fast food would then only have to check if the queue's frontmost customer has
`c->arrivalTime >= game->time.hours` , pop the customer, and in case
`game->time.checkHour(c->arrivalTime)` make the customer arrive.