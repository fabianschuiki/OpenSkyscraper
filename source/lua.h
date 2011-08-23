#pragma once

/* Include the Lua C headers. */
extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}


class LuaState {
public:
	LuaState();
	~LuaState();
	
	inline operator lua_State * () { return L; }
	
private:
	lua_State * L;
	
	static int lua_panic(lua_State * L);
};
