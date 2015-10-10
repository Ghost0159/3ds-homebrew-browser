#include <3ds.h>

#include "lua.hpp"
#include "ctrua/ctrua.hpp"

int main()
{
  lua_State* Lua = luaL_newstate();
  luaL_openlibs(Lua);
  ctrua::bind_ctrua(Lua);

  luaL_dostring(Lua,
    "srvInit()\n"
    "aptInit()\n"
    "fsInit()\n");

  Result rc = romfsInit();
  if (rc) {
    printf("romfsInit: %08lX\n", rc);
  } else {
    luaL_dofile(Lua, "main.lua");
  }

  // Main loop
  // while (aptMainLoop())
  // {
  //   gspWaitForVBlank();

  //   hidScanInput();
  //   u32 keys_down = hidKeysDown();
  //   touchPosition touch_position;
  //   hidTouchRead(&touch_position);

  //   if (keys_down & KEY_START) {
  //       aptSetStatus(APP_EXITING);
  //   }

  //   // Flush and swap framebuffers
  //   gfxFlushBuffers();
  //   gfxSwapBuffers();
  // }

  // Exit services
  SOC_Shutdown();
  httpcExit();
  fsExit();
  gfxExit();
  hidExit();
  aptExit();
  srvExit();
  lua_close(Lua);

  return 0;
}
