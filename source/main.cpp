#include <3ds.h>

#include "lua.hpp"
#include "ctrua/ctrua.hpp"



int main()
{
  // Initialize 3DS Services
  srvInit();
  aptInit();
  hidInit();
  gfxInitDefault();
  fsInit();
  httpcInit();

  // For now, give us a console so we can see basic Lua output
  // (later: Lua should have the ability to initialize its own console)
  consoleInit(GFX_TOP, nullptr);
  //*/

  lua_State* Lua = luaL_newstate();
  luaL_openlibs(Lua);

  bind_function(Lua, &gfxInitDefault);
  bind_function(Lua, VOID_WRAP(gspWaitForVBlank));

  luaL_dostring(Lua, "print \"Hello World! (Lua)\"");

  Result rc = romfsInit();
  if (rc) {
    printf("romfsInit: %08lX\n", rc);
  } else {
    luaL_dofile(Lua, "main.lua");
  }

  lua_close(Lua);

  printf("cplusplus: %ld", __cplusplus);

  // Main loop
  while (aptMainLoop())
  {
    gspWaitForVBlank();

    hidScanInput();
    u32 keys_down = hidKeysDown();
    touchPosition touch_position;
    hidTouchRead(&touch_position);

    if (keys_down & KEY_START) {
        aptSetStatus(APP_EXITING);
    }

    // Flush and swap framebuffers
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
  return 0;
}
