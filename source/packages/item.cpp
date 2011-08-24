#include <iostream>
#include "item.h"


void Item::expose(lua_State * L)
{
	//Create the Item class.
	LuaClass::make(L, "Item");
	
	//Expose the base functions.
	LuaExposable::expose<Item>(L);
	
	//Clean up.
	lua_pop(L, 1);
}

void Item::animate(double dt)
{
	//Load the animation function.
	loadFunction("animate");
	
	//Add the dt parameter.
	lua_pushnumber(L, dt);
	
	//Call the function.
	if (lua_pcall(L, 2, 0, 0) != 0)
		LuaError::report(L);
}

void Item::simulate(double dt)
{
	//Load the animation function.
	loadFunction("simulate");
	
	//Add the dt parameter.
	lua_pushnumber(L, dt);
	
	//Call the function.
	if (lua_pcall(L, 2, 0, 0) != 0)
		LuaError::report(L);
}
