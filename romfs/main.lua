--init = srvInit()
--aptInit()
ctru.hidInit()
ctru.gfxInitDefault()
--fsInit()
--httpcInit()
ctru.consoleInit(ctru.GFX_TOP)

soc_mem_size = 0x48000
soc_mem = ptr.memalign(0x1000, soc_mem_size)
error = ctru.SOC_Initialize(soc_mem, soc_mem_size);
print("Socket service initialization returned " .. error)

print "Hello from RomFS!"

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

ctru.errno(0)
s = ctru.socket(ctru.AF_INET, ctru.SOCK_STREAM, 0)
errno = ctru.errno()
print("socket opening returned", s)
print("errno is", errno)
print("strerror is", ctru.strerror(errno));
print("ENODEV:", ctru.ENODEV, "ENOMEM:", ctru.ENOMEM)

function put_pixel(x, y)
  fb, width, height = ctru.gfxGetFramebuffer(ctru.GFX_BOTTOM)
  ptr.setByte(fb, 3 * (width - y + x * width) + 0, 0xFF)
  ptr.setByte(fb, 3 * (width - y + x * width) + 1, 0xFF)
  ptr.setByte(fb, 3 * (width - y + x * width) + 2, 0xFF)
end

running = ctru.aptMainLoop()
while running do
  ctru.gspWaitForVBlank()

  ctru.hidScanInput()
  keys_down = ctru.hidKeysHeld()
  touch_position = ctru.hidTouchRead()
  --print(string.format("\x1b[0;20H%08x", keys_down))
  --print(string.format("\x1b[1;20H%04x %04x", touch_position.px, touch_position.py))
  if keys_down & ctru.KEY_START ~= 0 then
    print "start pressed!"
    ctru.aptSetStatus(ctru.APP_EXITING)
  end

  --graphics.put_pixel(touch_position.px, touch_position.py)
  --graphics.draw_image(an_better_icon, touch_position.px, touch_position.py)

  --font.draw_character("ubuntu_light_10pt", "A", 100, 100)
  --font.draw_string("ubuntu_light_10pt", "Hello World!", 100, 150)
  ui.draw()

  ctru.gfxFlushBuffers()
  ctru.gfxSwapBuffers()
  continue = ctru.aptMainLoop()
end
