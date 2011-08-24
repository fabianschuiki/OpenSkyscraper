#include <cassert>
#include "sprite.h"


void Sprite::expose(lua_State * L)
{
	//Create the Sprite class and expose the base functions.
	LuaClass::make(L, "Sprite");
	LuaExposable<Sprite>::expose(L);
	
	//Register the exposed functions.
	const static luaL_Reg functions[] = {
		{"setPosition", lua_setPosition},
		{NULL, NULL}
	};
	luaL_register(L, 0, functions);
	
	//Clean up.
	lua_pop(L, 1);
}

int Sprite::lua_setPosition(lua_State * L)
{
	int n = lua_gettop(L);
	if (n == 3) {
		Sprite * sprite = fromStack(L, 1);
		assert(sprite != NULL);
		sprite->SetPosition(luaL_checknumber(L, 2), luaL_checknumber(L, 3));
	} else {
		luaL_error(L, "Got %d arguments, expected 3 (self, x, y)", n);
	}
	return 0;
}
