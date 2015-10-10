#ifndef CTRUA_HPP
#define CTRUA_HPP

#include <3ds.h>
#include "lua.hpp"
#include <functional>
#include <string>

namespace ctrua {

void bind_ctrua(lua_State* Lua);

}  // namespace ctrua

#endif //CTRUA_HPP
