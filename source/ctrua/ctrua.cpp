#include "ctrua.hpp"

#include <tuple>
#include <vector>

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

int _lua_hidExit(lua_State* lvm) {
    hidExit();
    return 0;
}

int _lua_hidScanInput(lua_State* lvm) {
    hidScanInput();
    return 0;
}

int _lua_hidKeysHeld(lua_State* lvm) {
    auto const keys_held = hidKeysHeld();
    lua_pushinteger(lvm, keys_held);
    return 1;
}

int _lua_hidKeysDown(lua_State* lvm) {
    auto const keys_down = hidKeysDown();
    lua_pushinteger(lvm, keys_down);
    return 1;
}

int _lua_hidKeysUp(lua_State* lvm) {
    auto const keys_up = hidKeysUp();
    lua_pushinteger(lvm, keys_up);
    return 1;
}

int _lua_hidTouchRead(lua_State* lvm) {
    touchPosition touch{};
    hidTouchRead(&touch);
    lua_createtable(lvm, 0, 2);
    lua_pushinteger(lvm, touch.px);
    lua_setfield(lvm, -2, "px");
    lua_pushinteger(lvm, touch.py);
    lua_setfield(lvm, -2, "py");
    return 1;
}

int _lua_hidCircleRead(lua_State* lvm) {
    circlePosition stick{};
    hidCircleRead(&stick);
    lua_createtable(lvm, 0, 2);
    lua_pushinteger(lvm, stick.dx);
    lua_setfield(lvm, -2, "dx");
    lua_pushinteger(lvm, stick.dy);
    lua_setfield(lvm, -2, "dy");
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

#define BIND_CONSTANT(name) \
  lua_pushinteger(lvm, name); \
  lua_setfield(lvm, -2, #name)

#define BIND_FUNCTION(name) \
  lua_pushcfunction(lvm, _lua_##name); \
  lua_setfield(lvm, -2, #name)

void bind_hid(lua_State* lvm) {
    // enum PAD_KEY
    BIND_CONSTANT(KEY_A);
    BIND_CONSTANT(KEY_B);
    BIND_CONSTANT(KEY_SELECT);
    BIND_CONSTANT(KEY_START);
    BIND_CONSTANT(KEY_DRIGHT);
    BIND_CONSTANT(KEY_DLEFT);
    BIND_CONSTANT(KEY_DUP);
    BIND_CONSTANT(KEY_DDOWN);
    BIND_CONSTANT(KEY_R);
    BIND_CONSTANT(KEY_L);
    BIND_CONSTANT(KEY_X);
    BIND_CONSTANT(KEY_Y);
    BIND_CONSTANT(KEY_ZL);
    BIND_CONSTANT(KEY_ZR);
    BIND_CONSTANT(KEY_TOUCH);
    BIND_CONSTANT(KEY_CSTICK_RIGHT);
    BIND_CONSTANT(KEY_CSTICK_LEFT);
    BIND_CONSTANT(KEY_CSTICK_UP);
    BIND_CONSTANT(KEY_CSTICK_DOWN);
    BIND_CONSTANT(KEY_CPAD_RIGHT);
    BIND_CONSTANT(KEY_CPAD_LEFT);
    BIND_CONSTANT(KEY_CPAD_UP);
    BIND_CONSTANT(KEY_CPAD_DOWN);
    BIND_CONSTANT(KEY_UP);
    BIND_CONSTANT(KEY_DOWN);
    BIND_CONSTANT(KEY_LEFT);
    BIND_CONSTANT(KEY_RIGHT);

    BIND_FUNCTION(hidInit);
    BIND_FUNCTION(hidExit);

    BIND_FUNCTION(hidScanInput);
    BIND_FUNCTION(hidKeysHeld);
    BIND_FUNCTION(hidKeysDown);
    BIND_FUNCTION(hidKeysUp);
    BIND_FUNCTION(hidTouchRead);
    BIND_FUNCTION(hidCircleRead);
    // TODO: Accelerometer and gyroscope
}

namespace ctrua {

VOID_WRAP(gspWaitForVBlank);

void bind_ctrua(lua_State* lvm) {
    lua_newtable(lvm);

    bind_hid(lvm);

    lua_pushcfunction(lvm, _lua_srvInit);
    lua_setfield(lvm, -2, "srvInit");
    lua_pushcfunction(lvm, _lua_aptInit);
    lua_setfield(lvm, -2, "aptInit");
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
    lua_pushcfunction(lvm, _lua_gfxFlushBuffers);
    lua_setfield(lvm, -2, "gfxFlushBuffers");
    lua_pushcfunction(lvm, _lua_gfxSwapBuffers);
    lua_setfield(lvm, -2, "gfxSwapBuffers");
    lua_pushcfunction(lvm, _lua_aptSetStatus);
    lua_setfield(lvm, -2, "aptSetStatus");
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
