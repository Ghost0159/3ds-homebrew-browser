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
print "Hello again"

print("ctru.GFX_TOP " .. ctru.GFX_TOP)
print("ctru.GFX_BOTTOM " .. ctru.GFX_BOTTOM)

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

  put_pixel(touch_position.px, touch_position.py)

  ctru.gfxFlushBuffers()
  ctru.gfxSwapBuffers()
  continue = ctru.aptMainLoop()
end