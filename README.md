This is the experimental `restart` branch of OpenSkyscraper.

**You are welcome to join in the experiments, put forward suggestions or supply your own experimental code!**


How to try stuff
----------------
After you've cloned the repository, run `git submodule update --init .` inside your cloned
repository so the submodules are loaded appopriately.

To build the game, create a directory `build` and run `$ cmake ..` from within. Check that you have
all the necessary dependencies installed by looking at the CMake output warnings/errors. Once your
system checks out, run `$ make` to build, and `$ ./OpenSkyscraper` to launch the game. Observe the
game's output carefully as it may contain some hints why stuff isn't working.

At the moment, you need a `SIMTOWER.EXE` installed somewhere on your system. The game searches a
number of locations for this file. Check the `WindowsNEExecutable.cpp.15, load: from .../SIMTOWER.EXE`
lines of output for a list of locations.

If you instead wish to use the SIMTOWER.EX_ compressed from the disk, you must build with the optional
dependency of libmspack. As long as you have it installed in your system when you run "cmake .." then
it should be picked up.


What is being tested?
---------------------
Currently we're performing the following experiments and tests:

- **SFML** as platform API (instead of SDL)
  - loads bitmaps, fonts and sounds directly from memory
  - modern C++ interface simplifies code


Coding Style
------------

- Add a copyright line in the top comment of each file you edit:
  ```
  /* Copyright © 2010-2014 John Doe */
  /* Copyright © 2013 Mark Mensk */
  ```

- Everything resides in the OT namespace.

- Source file names match the class names in capitalization, e.g. the class `Application` yields `Application.{h|cpp}`.

- The directory hierarchy reflects the namespace hierarchy, so `Math::Vector2D` goes into `Math/Vector2D.{h|cpp}`.

There are some violations of this style left in the source. I'm working on getting rid of them.

*General note on source files:* Lowercase source files are likely to be outdated and removed in the future. They're probably there because they contain some working code snippet.
