--init = srvInit()
--aptInit()
hidInit()
gfxInitDefault()
--fsInit()
--httpcInit()
consoleInit(GFX_BOTTOM)

print "Hello from RomFS!"
print "Hello again"

print("GFX_TOP " .. GFX_TOP)
print("GFX_BOTTOM " .. GFX_BOTTOM)

running = aptMainLoop()
while running do
  --foalskdjf()
  --print "hi"
  --print(running)
  gspWaitForVBlank()

  hidScanInput()
  keys_down = hidKeysDown()
  print(keys_down)
  if keys_down & KEY_START ~= 0 then
    print "start pressed!"
    aptSetStatus(APP_EXITING)
  end

  gfxFlushBuffers()
  gfxSwapBuffers()
  continue = aptMainLoop()
end