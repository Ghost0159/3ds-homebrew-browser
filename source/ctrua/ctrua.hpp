#ifndef CTRUA_HPP
#define CTRUA_HPP

#include <3ds.h>
#include <functional>
#include <string>

#include "lua.hpp"

namespace ctrua {

void bind_ctrua(lua_State* lvm);
void bind_ptr(lua_State* lvm);

}  // namespace ctrua

#endif //CTRUA_HPP
