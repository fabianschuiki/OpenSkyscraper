#pragma once
#include <objlua/objlua.h>
#include "entity.h"


/** Sprite exposes various functions of the Entity class to Lua. */
class Sprite : public Entity, public LuaExposable<Sprite> {
public:
	OBJLUA_CONSTRUCTOR_WITH_CLASS_NAME(Sprite, NULL) {}
	static void expose(lua_State * L);
	
private:
	static int lua_setPosition(lua_State * L);
};
