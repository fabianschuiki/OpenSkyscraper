#include <iostream>
#include "../lua.h"
#include "room.h"





void Room::debug()
{
	LuaState L;
	
	luaL_dofile(L, "../Resources/debug/condo.lua");
	
	//Get the room structure that was created by the file.
	std::cout << "stack[0] type: " << lua_typename(L, lua_type(L, 0)) << "\n";
	lua_pop(L, 1);
}
