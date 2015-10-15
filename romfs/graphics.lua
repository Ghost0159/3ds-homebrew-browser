local graphics = {}

graphics.images = {}

function graphics.load_image(name)
  if graphics.images[name] then
    return graphics.images[name]
  end
  file = io.open("/images/" .. name .. ".bin")
  if not file then
    return nil
  end
  local file_data = file:read("*all")
  local image = {}
  image.width = string.unpack("I4", file_data)
  image.height = string.unpack("I4", file_data, 5)
  image.pixel_data = file_data:sub(9,file_data:len())
  graphics.images[name] = image
  io.close(file)
  return image
end

function graphics.image_pixel(image, x, y)
  local offset = (x * image.height + (image.height - y)) * 3 + 1
  local b, g, r = image.pixel_data:byte(offset, offset + 2)
  return r, g, b
end

function graphics.draw_image(image, screen_x, screen_y)
  ctru.draw_image_from_atlas(image, ctru.GFX_BOTTOM, screen_x, screen_y, 0, 0, image.width, image.height)
end

function graphics.draw_image_from_atlas(image, screen_x, screen_y, sub_x, sub_y, sub_width, sub_height)
  ctru.draw_image_from_atlas(image, ctru.GFX_BOTTOM, screen_x, screen_y, sub_x, sub_y, sub_width, sub_height)
end

function graphics.put_pixel(x, y, r, g, b)
  r = r or 0xFF
  g = g or 0xFF
  b = b or 0xFF
  fb, width, height = ctru.gfxGetFramebuffer(ctru.GFX_BOTTOM)
  ptr.setByte(fb, 3 * (width - y + x * width) + 0, b)
  ptr.setByte(fb, 3 * (width - y + x * width) + 1, g)
  ptr.setByte(fb, 3 * (width - y + x * width) + 2, r)
end

function graphics.fill_rect(x, y, width, height, r, g, b)
  ctru.fill_rect(ctru.GFX_BOTTOM, x, y, width, height, r, g, b)
end

return graphics
