local ui = {}
graphics = require("graphics")
font = require("font")

ui.defaults = {}

function ui.copy_table(input_table)
  local output_table = {}
  for k,v in pairs(input_table) do
    output_table[k] = v
  end
  return output_table
end

function ui.merge_tables(...)
  local output_table = {}
  local args = {...}
  --print("merge tables called")
  for i, input_table in ipairs(args) do
    --print("input table")
    for k,v in pairs(input_table) do
      --io.write(k .. ", ")
      output_table[k] = v
    end
    --io.write("\n")
  end
  return output_table
end

ui.defaults.element = {
  screen_x = 0,
  screen_y = 0,
  screen_width = 0,
  screen_height = 0,
  text = "",
  --children = {},
}

ui.defaults.region = {
  position = {x = 0, y = 0},
  width = 0,
  height = 0,
}

ui.layout = {
  type = "vertical_stack",
  width = 320,
  height = 240,
  background_color = {r = 255, g = 255, b = 0},
  children = {
    {
      type = "textarea",
      text = "Tomatoes",
      background_color = {r = 255, g = 128, b = 128},
      height = "50"
    },
    {
      type = "textarea",
      text = "Limes",
      background_color = {r = 128, g = 255, b = 128},
      height = "50"
    },
    {
      type = "textarea",
      text = "Blueberries",
      background_color = {r = 128, g = 128, b = 255},
      height = "50"
    },
  },
}

function ui.apply_defaults(element)
  local output_element = ui.merge_tables(ui.defaults.element, element)
  if ui.defaults[output_element.type] then
    output_element = ui.merge_tables(ui.defaults[output_element.type], output_element)
  end
  --repeat for this element's children
  if output_element.children then
    for i,_ in ipairs(output_element.children) do
      output_element.children[i] = ui.apply_defaults(output_element.children[i])
    end
  end
  return output_element
end

ui.layout = ui.apply_defaults(ui.layout)

function ui.calculate_screen_coordinates(element)
  if element.width then
    element.screen_width = element.width
  end
  if element.height then
    element.screen_height = element.height
  end

  if element.children then
    if element.type == "vertical_stack" then
      local y = element.screen_y
      for _,child in ipairs(element.children) do
        child.width = element.screen_width
        child.screen_x = element.screen_x
        child.screen_y = y
        y = y + child.height
      end
    elseif element.type == "horizontal_stack" then
      local x = element.screen_x
      for _,child in ipairs(element.children) do
        child.height = element.screen_height
        child.screen_x = x
        child.screen_y = element.screen_y
        x = x + child.width
      end
    else
      --Default case: ... I don't even know.
      print("Children of an unknown parent.")
    end
  end
end

function ui.draw_element(element)
  ui.calculate_screen_coordinates(element)
  local x = element.screen_x
  local y = element.screen_y
  local width = element.screen_width
  local height = element.screen_height
  if element.background_color then
    color = element.background_color
    graphics.fill_rect(x, y, width, height, color.r, color.g, color.b)
  end
  if element.text then
    font.draw_string("ubuntu_light_10pt", element.text, x, y)
  end
  if element.children then
    for _, child in ipairs(element.children) do
      ui.draw_element(child)
    end
  end
end

function ui.draw()
  graphics.fill_rect(0, 0, 320, 240, 0xFF, 0xFF, 0xFF)
  ui.draw_element(ui.layout)
end

return ui
