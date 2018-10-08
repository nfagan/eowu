local state = {}

state.Name = 'state2'
state.Duration = 1000

function state.Entry()
  local script = eowu.script()
  local state = script:State('state2')

  script:Render('state2')
  state:Next('state1')

  print('Entered: ' .. state.name)
end

function state.Exit()
  local state = eowu.script():State('state2')

  print('Exited: ' .. state.name)
end

local function state2_render()
  local script = eowu.script()
  local kb = script:Keyboard()
  local s1 = script:Stimulus('s1')

  s1:Color({1, 1, 1})

  if kb:Down('space') then
    s1:ZRotate(-0.05)
  else
    s1:ZRotate(0.01)
  end

  s1:Draw()
end

state.Render = {
  state2 = state2_render
}

return state