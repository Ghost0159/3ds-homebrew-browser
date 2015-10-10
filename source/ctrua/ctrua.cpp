#include "ctrua.hpp"

#define NAMED_PAIR(some_variable) #some_variable, &some_variable
#define VOID_WRAP(macro) void macro##_wrapped() {macro();}

template <void(*F)()>
void bind_function(lua_State* lvm, const char* name) {
  auto wrapped_function = [](lua_State* lvm)->int{
    F();
    return 0;
  };
  lua_pushcfunction(lvm, wrapped_function);
  lua_setglobal(lvm, name);
}

int _lua_srvInit(lua_State* lvm) {
    auto const result = srvInit();
    lua_pushinteger(lvm, result);
    return 1;
}

int _lua_aptInit(lua_State* lvm) {
    auto const result = aptInit();
    lua_pushinteger(lvm, result);
    return 1;
}

int _lua_hidInit(lua_State* lvm) {
    auto const result = hidInit();
    lua_pushinteger(lvm, result);
    return 1;
}

int _lua_gfxInitDefault(lua_State* lvm) {
    gfxInitDefault();
    return 0;
}

int _lua_fsInit(lua_State* lvm) {
    auto const result = fsInit();
    lua_pushinteger(lvm, result);
    return 1;
}

int _lua_httpcInit(lua_State* lvm) {
    auto const result = httpcInit();
    lua_pushinteger(lvm, result);
    return 1;
}

int _lua_consoleInit(lua_State* lvm) {
    auto const argc = lua_gettop(lvm);
    if(argc < 1) {
        return luaL_error(lvm, "expecting at least one argument");
    }
    if(2 < argc) {
        return luaL_error(lvm, "expecting at most two arguments");
    }
    if(argc == 2 and not lua_isnil(lvm, 2))
    {
        return luaL_error(lvm, "consoleInit second arg not implemented");
    }
    auto const screen = static_cast<gfxScreen_t>(lua_tointeger(lvm, 1));
    consoleInit(screen, nullptr);
    return 0;
}

int _lua_romfsInit(lua_State* lvm) {
    auto const result = romfsInit();
    lua_pushinteger(lvm, result);
    return 1;
}

int _lua_aptMainLoop(lua_State* lvm) {
    auto const should_continue = aptMainLoop();
    lua_pushboolean(lvm, should_continue);
    return 1;
}

int _lua_gspWaitForVBlank(lua_State* lvm) {
    gspWaitForVBlank();
    return 0;
}

int _lua_hidScanInput(lua_State* lvm) {
    hidScanInput();
    return 0;
}

int _lua_hidKeysDown(lua_State* lvm) {
    auto const keys_down = hidKeysDown();
    lua_pushinteger(lvm, keys_down);
    return 1;
}

int _lua_gfxFlushBuffers(lua_State* lvm) {
    gfxFlushBuffers();
    return 0;
}

int _lua_gfxSwapBuffers(lua_State* lvm) {
    gfxSwapBuffers();
    return 0;
}

int _lua_aptSetStatus(lua_State* lvm) {
    // auto const argc = lua_gettop(lvm);
    // TODO: error checking on argument count.
    auto const status = static_cast<APP_STATUS>(lua_tointeger(lvm, 1));
    aptSetStatus(status);
    return 0;
}

namespace ctrua {

VOID_WRAP(gspWaitForVBlank);

void bind_ctrua(lua_State* lvm) {
    lua_newtable(lvm);

    lua_pushcfunction(lvm, _lua_srvInit);
    lua_setfield(lvm, -2, "srvInit");
    lua_pushcfunction(lvm, _lua_aptInit);
    lua_setfield(lvm, -2, "aptInit");
    lua_pushcfunction(lvm, _lua_hidInit);
    lua_setfield(lvm, -2, "hidInit");
    lua_pushcfunction(lvm, _lua_gfxInitDefault);
    lua_setfield(lvm, -2, "gfxInitDefault");
    lua_pushcfunction(lvm, _lua_fsInit);
    lua_setfield(lvm, -2, "fsInit");

    lua_pushinteger(lvm, GFX_TOP);
    lua_setfield(lvm, -2, "GFX_TOP");
    lua_pushinteger(lvm, GFX_BOTTOM);
    lua_setfield(lvm, -2, "GFX_BOTTOM");
    lua_pushcfunction(lvm, _lua_consoleInit);
    lua_setfield(lvm, -2, "consoleInit");

    lua_pushcfunction(lvm, _lua_romfsInit);
    lua_setfield(lvm, -2, "romfsInit");

    lua_pushcfunction(lvm, _lua_aptMainLoop);
    lua_setfield(lvm, -2, "aptMainLoop");
    lua_pushcfunction(lvm, _lua_gspWaitForVBlank);
    lua_setfield(lvm, -2, "gspWaitForVBlank");
    lua_pushcfunction(lvm, _lua_hidScanInput);
    lua_setfield(lvm, -2, "hidScanInput");
    lua_pushcfunction(lvm, _lua_hidKeysDown);
    lua_setfield(lvm, -2, "hidKeysDown");
    lua_pushcfunction(lvm, _lua_gfxFlushBuffers);
    lua_setfield(lvm, -2, "gfxFlushBuffers");
    lua_pushcfunction(lvm, _lua_gfxSwapBuffers);
    lua_setfield(lvm, -2, "gfxSwapBuffers");
    lua_pushcfunction(lvm, _lua_aptSetStatus);
    lua_setfield(lvm, -2, "aptSetStatus");
    lua_pushinteger(lvm, KEY_START);
    lua_setfield(lvm, -2, "KEY_START");
    lua_pushinteger(lvm, APP_EXITING);
    lua_setfield(lvm, -2, "APP_EXITING");

    lua_setglobal(lvm, "ctru");

  // console
  //bind_function<consoleInit>(lvm, "consoleInit");
  // gfx
  bind_function<&gfxInitDefault>(lvm, "gfxInitDefault");

  // gsp
  bind_function<&gspWaitForVBlank_wrapped>(lvm, "gspWaitForVBlank");
}

}  //namespace ctrua
