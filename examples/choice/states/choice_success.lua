local state = {}
local state_id = 'choice_success'

local function entry()
  local script = eowu.script()
  local state = script:State(state_id)

  script:Render(state_id)
  state:Next('choice')

  print('Entered: ' .. state.name)
end

local function default_render()
  local script = eowu.script()
  local choice = script:State('choice')
  local selected = choice:Variable('selected'):Get()

  if selected == 't1' then
    script:Stimulus('s1'):Draw()
  elseif selected == 't2' then
    script:Stimulus('s2'):Draw()
  end
end

state.Entry = entry

state.Name = state_id
state.Duration = 1000
state.Render = {
  [state_id] = default_render
}

return state