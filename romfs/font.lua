local font = {}
local graphics = require("graphics")

font.loaded_fonts = {}

function font.load(name)
  if font.loaded_fonts[name] then
    return font.loaded_fonts[name]
  end

  local new_font = {}
  new_font.atlas = graphics.load_image(name)
  if not new_font.atlas then
    print("Can't load font " .. name .. ", no atlas file found!")
    return nil
  end

  local count = 0
  new_font.characters = {}
  for line in io.lines("/fonts/" .. name .. ".fnt") do
    if line:sub(1, 5) == "char " then
      -- split on whitespace to grab every key/value pair for this char
      char_params = {}
      for word in line:sub(6):gmatch("%S+") do
        divider = word:find("=")
        if divider then
          key = word:sub(1, divider - 1)
          value = word:sub(divider + 1)
          char_params[key] = tonumber(value)
        end
      end
      if char_params.id then
        new_font.characters[char_params.id] = char_params
      else
        print("Weird: read in a char without an ID parameter?")
      end
      count = count + 1
    end
  end
  print("Loaded " .. name .. " reading in " .. count .. " characters.")
  font.loaded_fonts[name] = new_font
  return new_font
end

function font.draw_character(font_name, char, x, y)
  char_data = font.loaded_fonts[font_name].characters[char:byte()]
  if char_data then
    graphics.draw_image_from_atlas(graphics.images[font_name], x, y,
    char_data.x, char_data.y, char_data.width, char_data.height)
  end
end

function font.draw_string(font_name, str, x, y)
  font_data = font.loaded_fonts[font_name]
  for i = 1, #str do
    char = str:sub(i,i)
    char_data = font_data.characters[char:byte()]
    if char_data then
      font.draw_character(font_name, char, x + char_data.xoffset, y + char_data.yoffset)
      x = x + char_data.xadvance
    end
  end
  print("Drew " .. str .. " with " .. #str .. "length")
end

return font
