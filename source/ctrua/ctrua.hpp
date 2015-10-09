#include <3ds.h>
#include "lua.hpp"
#include <functional>

#define VOID_WRAP(function) (void(*)())([](){function();})

template <typename F>
void bind_function(lua_State* Lua, F function_pointer);

template <>
void bind_function<void(*)()>(lua_State* Lua, void(*function)()) {
  
}
