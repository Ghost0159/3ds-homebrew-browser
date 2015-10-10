#include "ctrua.hpp"

#define NAMED_PAIR(some_variable) #some_variable, &some_variable
#define VOID_WRAP(macro) void macro##_wrapped() {macro();}

template <void(*F)()>
void bind_function(lua_State* Lua, const char* name) {
  auto wrapped_function = [](lua_State* Lua)->int{
    F();
    return 0;
  };
  lua_pushcfunction(Lua, wrapped_function);
  lua_setglobal(Lua, name);
}

namespace ctrua {

VOID_WRAP(gspWaitForVBlank);

void bind_ctrua(lua_State* Lua) {
  // console
  //bind_function<consoleInit>(Lua, "consoleInit");
  // gfx
  bind_function<&gfxInitDefault>(Lua, "gfxInitDefault");

  // gsp
  bind_function<&gspWaitForVBlank_wrapped>(Lua, "gspWaitForVBlank");
}

}  //namespace ctrua
