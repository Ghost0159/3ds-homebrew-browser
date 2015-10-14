local ui = {}

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
  for i, input_table in ipairs(...) do
    for k,v in pairs(input_table) do
      output_table[k] = v
    end
  end
  return output_table
end

ui.defaults.region = {
  position = {x = 0,y = 0},
  size = {width = 0, height = 0},
  dpad_direction = "horizontal",
  children = {}
}

ui.defaults.textarea = ui.merge_tables(ui.defaults.region, {
  text=""
})

ui.layout = {
  type = "region",
  size = {width = 320, height = 240},
  children = {
    {type = "text",
    text = "Hello World!",
    size = {width = 320, height = 100}},
  },
}

function ui.apply_defaults(element)
  output_element = element
  if ui.defaults[element.type] ~= nil then
    output_element = ui.merge_tables(ui.defaults[element.type], element)
  end
  --repeat for this element's children
  if output_element.children ~= nil then
    for i,child in ipairs(output_elements.children) do
      output_elements.children[i] = ui.apply_defaults(output_elements.children[i])
    end
  end
  return output_element
end

ui.layout = ui.apply_defaults(ui.layout)

return ui
