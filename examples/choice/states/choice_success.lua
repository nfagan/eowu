local state = {}
local state_id = 'choice_success'

local function entry()
  local state = eowu.CurrentState()

  eowu.Render(state_id)
  state:Next('choice')

  print('Entered: ' .. state.name)
end

local function default_render()
  local choice = eowu.State('choice')
  local selected = choice:Variable('selected'):Get()

  if selected == 't1' then
    eowu.Stimulus('s1'):Draw()
  elseif selected == 't2' then
    eowu.Stimulus('s2'):Draw()
  end
end

state.Entry = entry

state.Name = state_id
state.Duration = 1000
state.Render = {
  [state_id] = default_render
}

return state