#pragma once
#include <objlua/objlua.h>


class Item : public LuaExposable {
public:
	Item(lua_State * L, const char * className = "Item", bool leaveOnStack = false)
	: LuaExposable(L, className, leaveOnStack) {}
	static void expose(lua_State * L);
	
	void animate(double dt);
	void simulate(double dt);
};
