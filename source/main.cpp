#include <3ds.h>

#include "lua.hpp"
#include "ctrua/ctrua.hpp"

int main()
{
  lua_State* Lua = luaL_newstate();
  luaL_openlibs(Lua);
  ctrua::bind_ctrua(Lua);
  ctrua::bind_ptr(Lua);

  luaL_dostring(Lua,
    "srvInit()\n"
    "aptInit()\n"
    "fsInit()\n");

  Result rc = romfsInit();
  if (rc) {
    gfxInitDefault();
    consoleInit(GFX_TOP, nullptr);
    printf("romfsInit: %08lX\n", rc);
  } else {
    auto const error = luaL_dofile(Lua, "main.lua");
    consoleInit(GFX_TOP, nullptr);
    if (error) {
      printf("Error in script: %s\n", lua_tostring(Lua, -1));
    }
  }

  printf("Lua exited... press Start to exit.\n");
  while (aptMainLoop())
  {
    gspWaitForVBlank();

    hidScanInput();
    u32 keys_down = hidKeysDown();
    if (keys_down & KEY_START) {
        aptSetStatus(APP_EXITING);
    }

    gfxFlushBuffers();
    gfxSwapBuffers();
  }

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
