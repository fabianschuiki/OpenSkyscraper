#include <iostream>
#include <sstream>
#include "package.h"
#include "../game.h"

void dumpStack(lua_State * L);
void dumpStack(lua_State * L)
{
	std::cout << "#stack = " << lua_gettop(L) << ", " << lua_typename(L, lua_type(L, -1)) << "\n";
}


Package::Package(Game * game) : game(game)
{
	lua_State * L = game->lua;
	
	//Load and run the package's main Lua file.
	luaL_dofile(L, "../Resources/data/debug/package.lua");
	
	//Move the returned package into the global scope so it is available to the Lua environment.
	lua_pushvalue(L, -1);
	lua_setglobal(L, "pkg");
	
	//Fetch the temporary table.
	int pkg = lua_gettop(L);
	std::vector<std::string> missing;
	
	//Extract the ID.
	lua_pushstring(L, "id");
	lua_gettable(L, pkg);
	if (!lua_isnil(L, -1))
		id = lua_tostring(L, -1);
	else
		missing.push_back("id");
	lua_pop(L, 1);
	
	//Extract the cell size.
	lua_pushstring(L, "cell_w"); lua_gettable(L, pkg);
	lua_pushstring(L, "cell_h"); lua_gettable(L, pkg);
	if (!lua_isnil(L, -2)) cell.x = (int)lua_tointeger(L, -2); else missing.push_back("cell_w");
	if (!lua_isnil(L, -1)) cell.y = (int)lua_tointeger(L, -1); else missing.push_back("cell_h");
	lua_pop(L, 2);
	
	//Get rid of the temporary pkg.
	lua_pop(L, pkg);
	
	//Dump the issues.
	if (!missing.empty())
		std::cerr << "package is missing";
	for (int i = 0; i < missing.size(); i++)
		std::cerr << (i > 0 ? ", " : " ") << missing[i];
	if (!missing.empty())
		std::cerr << "\n";
	
	//Dump the package.
	std::cout << "loaded package " << description() << "\n";
}

std::string Package::description() const
{
	std::stringstream s;
	s << id << " {\n";
	s << "\tcell = " << cell.description() << "\n";
	s << "}";
	return s.str();
}
