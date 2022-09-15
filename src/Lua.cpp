#include "Lua.hpp"

bool Lua::CheckLua(lua_State* L, int r)
{
	if (r != LUA_OK)
	{
		std::string errormsg = lua_tostring(L, -1);
		std::cout << errormsg << std::endl;
		return false;
	}
	return true;
}

void Lua::run()
{
	// The Lua Stack: individual elements of the stack is indexed.  Each element on the stack contains the data, as well as information describing the data.  so when calling lua_isnumber, you interrogate the box to see what the data holds (so C++ knows the type).

	// Stack is indexed from bottom up starting with 1.  0 = empty stack
	// Can also be indexed relative to the top of the stack.  Top = -1, .... all the way down.  when querying the -1 up there, its specifying whats on top of the stack.

	std::string cmd = "a = 1";
	std::string cmd2 = "b = a + 12 * 79 + math.sin(23.7)";

	lua_State* L = luaL_newstate();											// L represents a single instance of a lua virtual machine
	luaL_openlibs(L);

	if (CheckLua(L, luaL_dostring(L, cmd.c_str())))
	{
		lua_getglobal(L, "a");												// pushes data box with a to the top of the stack
		if (lua_isnumber(L, -1))											// queries the top of the stack (index -1)
		{
			float a_in_cpp = static_cast<float>(lua_tonumber(L, -1));
			std::cout << "a_in_cpp: " << a_in_cpp << std::endl;

		}
	}

	if (CheckLua(L, luaL_dostring(L, cmd2.c_str())))
	{
		lua_getglobal(L, "b");
		if (lua_isnumber(L, -1))
		{
			float b_in_cpp = static_cast<float>(lua_tonumber(L, -1));
			std::cout << "b_in_cpp: " << b_in_cpp << std::endl;
		}
	}


	if (CheckLua(L, luaL_dofile(L, "src/practiceLua.lua")))
	{
		lua_getglobal(L, "testLua");
		if (lua_isnumber(L, -1))
		{
			float testLua = static_cast<float>(lua_tonumber(L, -1));
			std::cout << "from file practiceLua.lua: " << testLua << std::endl;
		}
	}

	int idx1, idx2, idx3, idx4;

	if (CheckLua(L, luaL_dofile(L, "src/practiceLua.lua")))
	{
		lua_getglobal(L, "idx1");
		if (lua_isnumber(L, -1))
		{
			idx1 = static_cast<int>(lua_tonumber(L, -1));
		}

		lua_getglobal(L, "idx2");
		if (lua_isnumber(L, -1))
		{
			idx2 = static_cast<int>(lua_tonumber(L, -1));
		}

		lua_getglobal(L, "idx3");
		if (lua_isnumber(L, -1))
		{
			idx3 = static_cast<int>(lua_tonumber(L, -1));
		}

		lua_getglobal(L, "idx4");
		if (lua_isnumber(L, -1))
		{
			idx4 = static_cast<int>(lua_tonumber(L, -1));
		}
	}

	if (CheckLua(L, luaL_dofile(L, "src/practiceLua.lua")))
	{
		lua_getglobal(L, "Add");
		if (lua_isfunction(L, -1))
		{
			lua_pushnumber(L, 0.3f);
			lua_pushnumber(L, 0.5f);

			if (CheckLua(L, lua_pcall(L, 2, 1, 0)))
			{
				std::cout << "Lua: " << static_cast<float>(lua_tonumber(L, -1)) << std::endl;
			}

		}
	}

	int R1, G1;
	if (CheckLua(L, luaL_dofile(L, "src/practiceLua.lua")))
	{
		lua_getglobal(L, "color");
		if (lua_istable(L, -1))
		{
			lua_pushstring(L, "R1");
			lua_gettable(L, -2);
			R1 = lua_tonumber(L, -1);
			lua_pop(L, 1);

			lua_pushstring(L, "G1");
			lua_gettable(L, -2);
			G1 = lua_tonumber(L, -1);
			lua_pop(L, -1);

		}
	}
}

void Lua::close(lua_State *L)
{
	lua_close(L);
}




