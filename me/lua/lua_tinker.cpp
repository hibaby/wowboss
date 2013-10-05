
#include "lua_tinker.h"
#include "../misc.h"
#include <cassert>

namespace lua_tinker
{
	void print_error(lua_State *L, const char* fmt, ...)
	{
		char text[4096];
		
		va_list args;
		va_start(args, fmt);
		vsprintf(text, fmt, args);
		va_end(args);
		
		//fix5.2
		//lua_pushstring(L, "_ALERT");
		//lua_gettable(L, LUA_GLOBALSINDEX);
		lua_getglobal(L, "_ALERT");
		if(lua_isfunction(L, -1))
		{
			lua_pushstring(L, text);
			lua_call(L, 1, 0);
		}
		else
		{
			//printf("%s\n", text);
			me::dbg::error("%s\n", text);
			lua_pop(L, 1);
		}
		me::dbg::error("lua_tinker::print_error %s\n", text);
		me::dbg::save();
	}

	static void call_stack(lua_State* L, int n)
	{
		lua_Debug ar;
		if(lua_getstack(L, n, &ar) == 1)
		{
			lua_getinfo(L, "nSlu", &ar);
			
			const char* indent;
			if(n == 0)
			{
				indent = "->\t";
				print_error(L, "\t<call stack>");
			}
			else
			{
				indent = "\t";
			}
			
			if(ar.name)
				print_error(L, "%s%s() : line %d [%s : line %d]", indent, ar.name, ar.currentline, ar.source, ar.linedefined);
			else
				print_error(L, "%sunknown : line %d [%s : line %d]", indent, ar.currentline, ar.source, ar.linedefined);
			
			call_stack(L, n+1);
		}
	}

	int on_error(lua_State *L)
	{
		print_error(L, "%s", lua_tostring(L, -1));
		
		call_stack(L, 0);
		
		assert( false );
		
		return 0;	
	}
	
	void dofile(lua_State *L, const char *filename)
	{
		lua_pushcclosure(L, on_error, 0);
		int errfunc = lua_gettop(L);
		
		if(luaL_loadfile(L, filename) == 0)
		{
			if(lua_pcall(L, 0, 0, errfunc) != 0)
			{
				lua_pop(L, 1);
			}
		}
		else
		{
			print_error(L, "%s", lua_tostring(L, -1));
			lua_pop(L, 1);
		}
		
		lua_pop(L, 1);
	}

	void call(lua_State* L, const char* name)
	{
		lua_pushcclosure(L, on_error, 0);
		int errfunc = lua_gettop(L);
		
		//fix5.2
		//lua_pushstring(L, name);
		//lua_gettable(L, LUA_GLOBALSINDEX);
		lua_getglobal(L, name);
		if(lua_isfunction(L,-1))
		{
			if(lua_pcall(L, 0, 1, errfunc) != 0)
			{
				lua_pop(L, 1);
			}
		}
		else
		{
			print_error(L, "lua_tinker::call() attempt to call global `%s' (not a function)", name);
		}
		
		lua_remove(L, -2);
	}

} // namespace lua_tinker
