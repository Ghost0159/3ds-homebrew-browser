#include "ctrua.hpp"
#include <cstring>

#include <errno.h>
#include <malloc.h>

#include <cstring>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

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

int _lua_gfxGetFramebuffer(lua_State* lvm) {
    auto const screen = static_cast<gfxScreen_t>(lua_tointeger(lvm, 1));
    auto const side = static_cast<gfx3dSide_t>(lua_tointeger(lvm, 2));
    u16 width{};
    u16 height{};
    auto buffer = gfxGetFramebuffer(screen, side, &width, &height);
    lua_pushlightuserdata(lvm, buffer);
    lua_pushinteger(lvm, width);
    lua_pushinteger(lvm, height);
    return 3;
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

int _lua_errno(lua_State* lvm) {
    auto const argc = lua_gettop(lvm);
    if (argc < 0) {
        errno = lua_tointeger(lvm, 1);
        return 1;
    }
    lua_pushinteger(lvm, errno);
    return 1;
}

int _lua_strerror(lua_State* lvm) {
    auto const error_code = lua_tointeger(lvm, 1);
    auto const error_string = strerror(error_code);
    lua_pushstring(lvm, error_string);
    return 1;
}

int _lua_romfsInit(lua_State* lvm) {
    auto const result = romfsInit();
    lua_pushinteger(lvm, result);
    return 1;
}

int _lua_SOC_Initialize(lua_State* lvm) {
    auto const address = static_cast<u32*>(lua_touserdata(lvm, 1));
    auto const bytes = lua_tointeger(lvm, 2);
    auto const error = SOC_Initialize(address, bytes);
    lua_pushinteger(lvm, error);
    return 1;
}

int _lua_inet_addr(lua_State* lvm) {
    auto const hostname = lua_tostring(lvm, 1);
    auto const address = inet_addr(hostname);
    lua_pushinteger(lvm, address);
    return 1;
}

int _lua_htons(lua_State* lvm) {
    auto const host_order = static_cast<u16>(lua_tointeger(lvm, 1));
    auto const network_order = htons(host_order);
    lua_pushinteger(lvm, network_order);
    return 1;
}

int _lua_socket(lua_State* lvm) {
    auto const domain = lua_tointeger(lvm, 1);
    auto const type = lua_tointeger(lvm, 2);
    auto const protocol = lua_tointeger(lvm, 3);
    printf("calling socket(%d, %d, %d)\n", domain, type, protocol);
    auto const socket_descriptor = socket(domain, type, protocol);
    lua_pushinteger(lvm, socket_descriptor);
    return 1;
}

int _lua_connect(lua_State* lvm) {
    fprintf(stderr, "0");
    auto const socket_descriptor = lua_tointeger(lvm, 1);
    fprintf(stderr, "1");
    sockaddr_in server_address{};
    lua_getfield(lvm, 2, "sin_addr");
    fprintf(stderr, "2");
    lua_getfield(lvm, -1, "s_addr");
    fprintf(stderr, "3");
    server_address.sin_addr.s_addr = lua_tointeger(lvm, -1);
    lua_pop(lvm, 2);
    lua_getfield(lvm, 2, "sin_port");
    fprintf(stderr, "4");
    server_address.sin_port = lua_tointeger(lvm, -1);
    lua_pop(lvm, 1);
    lua_getfield(lvm, 2, "sin_family");
    fprintf(stderr, "5");
    server_address.sin_family = lua_tointeger(lvm, -1);

    fprintf(stderr, "connect(%d, {sin_addr = {s_addr = %X}, sin_port = %04X, sin_family = %d}, %u)\n", socket_descriptor, static_cast<unsigned int>(server_address.sin_addr.s_addr), server_address.sin_port, server_address.sin_family, sizeof(server_address));
    auto const error = connect(socket_descriptor,
        reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address));
    //return 0;
    lua_pushinteger(lvm, error);
    return 1;
}

int _lua_srvInit(lua_State* lvm) {
    auto const result = srvInit();
    lua_pushinteger(lvm, result);
    return 1;
}

int _lua_memalign(lua_State* lvm) {
    // TODO: sanity checking on the sizes passed.
    auto const alignment = lua_tointeger(lvm, 1);
    auto const bytes = lua_tointeger(lvm, 2);
    auto const memory = memalign(alignment, bytes);
    lua_pushlightuserdata(lvm, memory);
    return 1;
}

int _lua_setByte(lua_State* lvm) {
    // TODO: Bounds checking on the arguments
    u8* const address = static_cast<u8*>(lua_touserdata(lvm, 1));
    auto const offset = lua_tointeger(lvm, 2);
    auto const value = lua_tointeger(lvm, 3);
    address[offset] = value;
    return 0;
}

int _lua_isNull(lua_State* lvm) {
    auto const address = lua_touserdata(lvm, 1);
    bool const is_null = address == nullptr;
    lua_pushboolean(lvm, is_null);
    return 1;
}

int _lua_fill_rect(lua_State* lvm) {
  auto const screen = static_cast<gfxScreen_t>(lua_tointeger(lvm, 1));
  auto const rect_x = lua_tointeger(lvm, 2);
  auto const rect_y = lua_tointeger(lvm, 3);
  auto const rect_width = lua_tointeger(lvm, 4);
  auto const rect_height = lua_tointeger(lvm, 5);
  auto const r = lua_tointeger(lvm, 6);
  auto const g = lua_tointeger(lvm, 7);
  auto const b = lua_tointeger(lvm, 8);

  u16 fb_width, fb_height;
  auto fb = gfxGetFramebuffer(screen, GFX_LEFT, &fb_height, &fb_width);

  const int pixel_width = 3;


  for (int x = 0; x < rect_width; x++) {
    int x_offset = (rect_x + x) * fb_height * pixel_width;
    for (int y = 0; y < rect_height; y++) {
      int y_offset = (fb_height - rect_y - rect_height + y) * pixel_width;
      fb[x_offset + y_offset + 0] = b;
      fb[x_offset + y_offset + 1] = g;
      fb[x_offset + y_offset + 2] = r;
    }
  }

  return 0;
}

int _lua_draw_image_from_atlas(lua_State* lvm) {
  // Read image data from the provided table
  lua_getfield(lvm, 1, "height");
  auto const image_height = lua_tointeger(lvm, -1);
  lua_pop(lvm, 1);

  lua_getfield(lvm, 1, "pixel_data");
  auto const pixel_data = lua_tolstring(lvm, -1, nullptr);
  lua_pop(lvm, 1);

  auto const screen = static_cast<gfxScreen_t>(lua_tointeger(lvm, 2));
  auto const screen_x = lua_tointeger(lvm, 3);
  auto const screen_y = lua_tointeger(lvm, 4);
  auto const sub_x = lua_tointeger(lvm, 5);
  auto const sub_y = lua_tointeger(lvm, 6);
  auto const sub_width = lua_tointeger(lvm, 7);
  auto const sub_height = lua_tointeger(lvm, 8);

  u16 fb_width, fb_height;
  auto fb = gfxGetFramebuffer(screen, GFX_LEFT, &fb_height, &fb_width);

  int y_offset = fb_height - screen_y - sub_height;
  for (s32 x = 0; x < sub_width; x++) {
    int x_offset = screen_x * fb_height + x * fb_height;
    const int pixel_width = 3;
    auto destination = fb + (x_offset + y_offset) * pixel_width;
    auto source = pixel_data + ((sub_x + x) * image_height + (image_height - sub_y - sub_height)) * pixel_width;
    memcpy(destination, source, sub_height * pixel_width);
  }
  return 0;
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

    // enum gfx3dSide_t
    BIND_CONSTANT(GFX_LEFT);
    BIND_CONSTANT(GFX_RIGHT);

    BIND_FUNCTION(gfxInitDefault);
    BIND_FUNCTION(gfxFlushBuffers);
    BIND_FUNCTION(gfxSwapBuffers);
    BIND_FUNCTION(gfxGetFramebuffer);

    // project specific bindings
    BIND_FUNCTION(draw_image_from_atlas);
    BIND_FUNCTION(fill_rect);
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

void bind_posix(lua_State* lvm) {
    BIND_CONSTANT(ENODEV);
    BIND_CONSTANT(ENOMEM);

    BIND_FUNCTION(errno);
    BIND_FUNCTION(strerror);
}

void bind_romfs(lua_State* lvm) {
    BIND_FUNCTION(romfsInit);
}

void bind_soc(lua_State* lvm) {
    BIND_CONSTANT(AF_INET);
    BIND_CONSTANT(SOCK_STREAM);

    BIND_FUNCTION(SOC_Initialize);

    BIND_FUNCTION(inet_addr);
    BIND_FUNCTION(htons);
    BIND_FUNCTION(socket);
    BIND_FUNCTION(connect);
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
    bind_posix(lvm);
    bind_romfs(lvm);
    bind_soc(lvm);
    bind_srv(lvm);

    lua_setglobal(lvm, "ctru");
}

void bind_ptr(lua_State* lvm) {
    lua_newtable(lvm);

    BIND_FUNCTION(memalign);

    BIND_FUNCTION(setByte);
    BIND_FUNCTION(isNull);

    lua_setglobal(lvm, "ptr");
}

}  // namespace ctrua
