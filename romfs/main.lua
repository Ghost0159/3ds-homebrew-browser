--init = srvInit()
--aptInit()
hidInit()
gfxInitDefault()
--fsInit()
--httpcInit()
consoleInit(GFX_BOTTOM)

print "Hello from RomFS!"
print "Hello again;"

print("GFX_TOP " .. GFX_TOP)
print("GFX_BOTTOM " .. GFX_BOTTOM)

--while aptMainLoop() do
while true do
  gspWaitForVBlank()

  hidScanInput()
  keys_down = hidKeysDown()
  if keys_down & KEY_START then
    aptSetStatus(APP_EXITING)
  end

  gfxFlushBuffers()
  gfxSwapBuffers()
end