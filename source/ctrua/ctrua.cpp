#include "ctrua.hpp"

#define BIND_CONSTANT(name) \
  lua_pushinteger(lvm, name); \
  lua_setfield(lvm, -2, #name)

#define BIND_FUNCTION(name) \
  lua_pushcfunction(lvm, _lua_##name); \
  lua_setfield(lvm, -2, #name)

namespace {

int _lua_aptInit(lua_State* lvm) {
    auto const result = aptInit();
    lua_pushinteger(lvm, result);
    return 1;
}

int _lua_aptSetStatus(lua_State* lvm) {
    // auto const argc = lua_gettop(lvm);
    // TODO: error checking on argument count.
    auto const status = static_cast<APP_STATUS>(lua_tointeger(lvm, 1));
    aptSetStatus(status);
    return 0;
}

int _lua_aptMainLoop(lua_State* lvm) {
    auto const should_continue = aptMainLoop();
    lua_pushboolean(lvm, should_continue);
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

int _lua_fsInit(lua_State* lvm) {
    auto const result = fsInit();
    lua_pushinteger(lvm, result);
    return 1;
}

int _lua_gfxInitDefault(lua_State* lvm) {
    gfxInitDefault();
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

int _lua_gspWaitForVBlank(lua_State* lvm) {
    gspWaitForVBlank();
    return 0;
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

int _lua_httpcInit(lua_State* lvm) {
    auto const result = httpcInit();
    lua_pushinteger(lvm, result);
    return 1;
}

int _lua_romfsInit(lua_State* lvm) {
    auto const result = romfsInit();
    lua_pushinteger(lvm, result);
    return 1;
}

int _lua_srvInit(lua_State* lvm) {
    auto const result = srvInit();
    lua_pushinteger(lvm, result);
    return 1;
}

void bind_apt(lua_State* lvm) {
    // enum APP_STATUS
    BIND_CONSTANT(APP_EXITING);

    BIND_FUNCTION(aptInit);
    BIND_FUNCTION(aptSetStatus);
    BIND_FUNCTION(aptMainLoop);
}

void bind_console(lua_State* lvm) {
    BIND_FUNCTION(consoleInit);
}

void bind_fs(lua_State* lvm) {
    BIND_FUNCTION(fsInit);
}

void bind_gfx(lua_State* lvm) {
    // enum gfxScreen_t
    BIND_CONSTANT(GFX_TOP);
    BIND_CONSTANT(GFX_BOTTOM);

    BIND_FUNCTION(gfxInitDefault);
    BIND_FUNCTION(gfxFlushBuffers);
    BIND_FUNCTION(gfxSwapBuffers);
}

void bind_gsp(lua_State* lvm) {
    BIND_FUNCTION(gspWaitForVBlank);
}

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

void bind_httpc(lua_State* lvm) {
    BIND_FUNCTION(httpcInit);
}

void bind_romfs(lua_State* lvm) {
    BIND_FUNCTION(romfsInit);
}

void bind_srv(lua_State* lvm) {
    BIND_FUNCTION(srvInit);
}

}  // namespace

namespace ctrua {

void bind_ctrua(lua_State* lvm) {
    lua_newtable(lvm);

    bind_apt(lvm);
    bind_console(lvm);
    bind_fs(lvm);
    bind_gfx(lvm);
    bind_gsp(lvm);
    bind_hid(lvm);
    bind_httpc(lvm);
    bind_romfs(lvm);
    bind_srv(lvm);

    lua_setglobal(lvm, "ctru");
}

}  // namespace ctrua
