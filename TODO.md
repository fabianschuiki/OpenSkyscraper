OpenSkyscraper TODO
===================

Background
----------

- Put background drawing stuff into its own `Background` class


Tower
-----

### Drawing
- Reorganize tower stuff into *game/tower* directory
- Draw the fire ladders on the far left and right of each floor; also on the lobby floor. (they're in 075)
- Add clouds to the sky
- Add *funds*, *star rating*, *time* and *date* to the `Tower` class and animate them


Items
-----

- When two adjacent items are collapsed, they both show the construction animation. It should be possible to only show the animation for the part that was actually added to cause the collapse.


Lobby
-----

- Multiple unconnected lobbies on the ground floor allowed. This should *not* be the case!
- `Lobby` needs a texture (don't know where to find it though…)


User Interface
--------------

- Figure out a way to draw the UI (windows, sprites, entities, hierarchies, etc.)
- Create `Tools`, `Map` and `Control` windows that also were in the original SimTower.


Audio
-----

- Initialize OpenAL or whatever we will use for audio
- Create `Sound` class
- Create `SoundEmitter` class
- Locate listener at the center of the screen (POI of the `TowerScene`)
