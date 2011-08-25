#pragma once
#include <objlua/objlua.h>
#include "entity.h"


/** Sprite exposes various functions of the Entity class to Lua. */
class Sprite : public Entity, public LuaExposable<Sprite> {
public:
	OBJLUA_CONSTRUCTOR(Sprite) {}
	Sprite(lua_State * L, const char * className);
	static void expose(lua_State * L);
	
private:
	static int lua_setPosition(lua_State * L);
};
