--init = srvInit()
--aptInit()
ctru.hidInit()
ctru.gfxInitDefault()
--fsInit()
--httpcInit()
ctru.consoleInit(ctru.GFX_BOTTOM)

print "Hello from RomFS!"
print "Hello again"

print("ctru.GFX_TOP " .. ctru.GFX_TOP)
print("ctru.GFX_BOTTOM " .. ctru.GFX_BOTTOM)

running = ctru.aptMainLoop()
while running do
  --foalskdjf()
  --print "hi"
  print(running)
  ctru.gspWaitForVBlank()

  ctru.hidScanInput()
  keys_down = ctru.hidKeysDown()
  --print(keys_down)
  if keys_down & ctru.KEY_START ~= 0 then
    print "start pressed!"
    ctru.aptSetStatus(ctru.APP_EXITING)
  end

  ctru.gfxFlushBuffers()
  ctru.gfxSwapBuffers()
  continue = ctru.aptMainLoop()
end