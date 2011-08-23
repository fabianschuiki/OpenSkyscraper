#include <iostream>
#include "lua.h"


LuaState::LuaState()
{
	//Initialize the Lua state.
	L = lua_open();
	if (!L) {
		std::cerr << "Cannot open Lua state.\n";
		return;
	}
	
	//Setup the basic panic function so we at least know when Lua goes berserk.
	lua_atpanic(L, LuaState::lua_panic);
	
	//Load the default libraries.
	luaL_openlibs(L);
}

LuaState::~LuaState()
{
	lua_close(L);
	L = NULL;
}

int LuaState::lua_panic(lua_State * L)
{
	//Dump the general warning.
	std::cerr << "*** A Lua error has occured outside of protected mode! ***\n";
	std::cerr.flush();
	
	//Dump details.
	if (lua_type(L, -1) == LUA_TSTRING)
		std::cerr << lua_tostring(L, -1) << "\n";
	else
		std::cerr << lua_topointer(L, -1) << "\n";
	std::cerr.flush();
	
	return 0;
}
