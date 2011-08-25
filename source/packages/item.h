#pragma once
#include <set>
#include <objlua/objlua.h>

class Sprite;
class Tower;


/** Base class for all items that can be built in a tower. The Item class is exposed to Lua and re-
 lies on Lua scripts to subclass Item and provide implementations for various functions.
 
 The following functions need to be implemented in Lua:
 - animate
 - simulate
 
 The following functions are available in Lua:
 - addSprite
 - removeSprite
 */
class Item : public LuaExposable<Item> {
	friend class LuaExposable<Item>;
	
public:
	Tower * const tower;
	
	OBJLUA_CONSTRUCTOR_WITH_CLASS_NAME(Item, NULL), tower(NULL) {}
	Item(Tower * tower, const char * className = "Item");
	~Item();
	static void expose(lua_State * L);
	
	/** Advances the item's animation by the given amount of time by calling the object's Lua
	 implementation of the function. */
	void animate(double dt);
	/** Advances the item's simulation by the given amount of time by calling the object's Lua
	 implementation of the function. */
	void simulate(double dt);
	
	/** Adds the sprite to this item. Also adds the sprite to the item's parent tower. The Item
	 class takes over responsibility for deleting the sprite. */
	void addSprite(Sprite * s);
	/** Removes the sprite from this item. Also removes the sprite from the item's parent tower. */
	void removeSprite(Sprite * s);
	
private:
	std::set<Sprite *> sprites;
	
protected:
	//static int lua_new(lua_State * L);
	
	static int lua_addSprite(lua_State * L);
	static int lua_removeSprite(lua_State * L);
};
