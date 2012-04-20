- create `CommmandLineArguments` class which handles interpretation of the command line arguments
  (e.g. `--dump-simtower <path>`)
- create `FileSystem` class to do file manipulation (move, copy, mkdir, etc.)
	- adjust `WindowsNEExecutable::dump`
- In `Route`, keep track of how many stairs and elevators were used. Then make the pathfinder limit
  the amount of stairs and elevators that are used per route. Otherwise algorithm complexity will
  explode.

Game
----

- `mapWindow` should be its own class `MapWindow`, like the other two.

Item::FastFood
--------------
In order to make the customers arrive at the tower, the fast food item has to iterate over all its
customers at each frame. This could be made more efficient if there was an arrival queue which,
after today's customers are initialized, is populated with the customers ordered by their arrival
time. The fast food would then only have to check if the queue's frontmost customer has
`c->arrivalTime >= game->time.hours` , pop the customer, and in case
`game->time.checkHour(c->arrivalTime)` make the customer arrive.