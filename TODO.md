OpenSkyscraper TODO
===================


Tower
-----

- Implement a pathfinding algorithm to find routes from one facility to another.
- Add some methods to find the closest transport item.
- Draw the fire ladders on the far left and right of each floor; also on the lobby floor. (they're in 075)
- Add clouds to the sky


Items
-----

- When two adjacent items are collapsed, they both show the construction animation. It should be possible to only show the animation for the part that was actually added to cause the collapse.
- Create classes for all items of SimTower.
- Implement transport facility's capability to move people.
- Add people to the office item and animate them appropriately.
- Add people to all other items.


People
------

- Create a People class.
- Make the People class use the pathfinding algorithm of the Tower class to find its route through the tower.


Lobby
-----

- Multiple unconnected lobbies on the ground floor allowed. This should *not* be the case!
- `Lobby` needs a texture (don't know where to find it though…)


User Interface
--------------

- Create `Tools`, `Map` and `Control` windows that also were in the original SimTower.
