#include <3ds.h>

#include "lua.hpp"
//#include "luwra.hpp"

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

  luaL_dostring(Lua, "print \"Hello World! (Lua)\"");
  lua_close(Lua);

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
