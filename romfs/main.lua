--init = srvInit()
--aptInit()
ctru.hidInit()
ctru.gfxInitDefault()
--fsInit()
--httpcInit()
ctru.consoleInit(ctru.GFX_TOP)

print "Hello from RomFS!"
print "Hello again"

print("ctru.GFX_TOP " .. ctru.GFX_TOP)
print("ctru.GFX_BOTTOM " .. ctru.GFX_BOTTOM)

ui = require("ui")
graphics = require("graphics")
font = require("font")

local an_better_icon = graphics.load_image("an_better_icon")
local ubuntu_light_10pt = font.load("ubuntu_light_10pt")

--initialize us to a plain white screen
graphics.fill_rect(0, 0, 320, 240, 0xFF, 0xFF, 0xFF)
ctru.gfxFlushBuffers()
ctru.gfxSwapBuffers()
graphics.fill_rect(0, 0, 320, 240, 0xFF, 0xFF, 0xFF)

running = ctru.aptMainLoop()
while running do
  --foalskdjf()
  --print "hi"
  --print(running)
  ctru.gspWaitForVBlank()

  ctru.hidScanInput()
  keys_down = ctru.hidKeysHeld()
  touch_position = ctru.hidTouchRead()
  print(string.format("\x1b[0;20H%08x", keys_down))
  print(string.format("\x1b[1;20H%04x %04x", touch_position.px, touch_position.py))
  if keys_down & ctru.KEY_START ~= 0 then
    print "start pressed!"
    ctru.aptSetStatus(ctru.APP_EXITING)
  end

  graphics.put_pixel(touch_position.px, touch_position.py)
  graphics.draw_image(an_better_icon, touch_position.px, touch_position.py)

  font.draw_character("ubuntu_light_10pt", "A", 100, 100)
  font.draw_string("ubuntu_light_10pt", "Hello World!", 100, 150)

  ctru.gfxFlushBuffers()
  ctru.gfxSwapBuffers()
  continue = ctru.aptMainLoop()
end
