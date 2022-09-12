#pragma once
#include <string>
#include <iostream>
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#pragma comment(lib, "liblua54.a")


class Lua
{
public:

	void run();
	void close(lua_State *L);
	bool CheckLua(lua_State* L, int r);

private:
	lua_State* L;
};



