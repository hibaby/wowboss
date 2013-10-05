#if !defined(_LUA_TINKER_H_)
#define _LUA_TINKER_H_
#include "lua.hpp"

namespace lua_tinker
{
	void print_error(lua_State *L, const char* fmt, ...);
	int on_error(lua_State *L);
	
	void dofile(lua_State *L, const char *filename);
	void call(lua_State* L, const char* name);

} // namespace lua_tinker
#endif //_LUA_TINKER_H_
