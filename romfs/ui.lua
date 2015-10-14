defaults = {}

function copy_table(input_table)
  local output_table = {}
  for k,v in pairs(input_table) do
    output_table[k] = v
  end
  return output_table
end

function merge_tables(...)
  local output_table = {}
  for i, input_table in ipairs(...) do
    for k,v in pairs(input_table) do
      output_table[k] = v
    end
  end
  return output_table
end

defaults.region = {
  position = {x = 0,y = 0},
  size = {width = 0, height = 0},
  dpad_direction = "horizontal",
  children = {}
}

defaults.textarea = merge_tables(defaults.region, {
  text=""
})

layout = {
  type = "region",
  size = {width = 320, height = 240},
  children = {
    {type = "text",
    text = "Hello World!",
    size = {width = 320, height = 100}},
  },
}

function apply_defaults(element)
  output_element = element
  if defaults[element.type] ~= nil then
    output_element = merge_tables(defaults[element.type], element)
  end
  --repeat for this element's children
  if output_element.children ~= nil then
    for i,child in ipairs(output_elements.children) do
      output_elements.children[i] = apply_defaults(output_elements.children[i])
    end
  end
  return output_element
end

layout = apply_defaults(layout)
