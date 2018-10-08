local state = {}
local state_id = 'choice_error'

local function entry()
  local script = eowu.script()
  local state = script:State(state_id)
  
  state:Next('choice')
  script:Render(state_id)

  print('Entered: ' .. state.name)
end

local function default_render()
  eowu.script():Stimulus('s3'):Draw()
end

state.Entry = entry

state.Duration = 1000
state.Name = state_id
state.Render = {
  [state_id] = default_render
}

return state