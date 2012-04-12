- create `CommmandLineArguments` class which handles interpretation of the command line arguments (e.g. `--dump-simtower <path>`)
- create `FileSystem` class to do file manipulation (move, copy, mkdir, etc.)
	- adjust `WindowsNEExecutable::dump`

Game
----

- `mapWindow` should be its own class `MapWindow`, like the other two.