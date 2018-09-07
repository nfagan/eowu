local state = {}

state.ID = 'fixation'
state.Duration = 1000
state.First = true

function state.Entry()
  print('Entered fixation!')
  eowu_script():State('fixation'):Next('fixation')
  eowu_script():Render('fixation')
end

function state.Exit()
  print(eowu_script():State('fixation'):Ellapsed())
end

function state.Loop()
end

local idx = 1

local function render_default()
  local stim = eowu_script():Stimulus('sq')
  stim:Units('normalized')
  stim:Color(1, 0, 0)

  local scale = stim.scale
  local pos = stim.position
  local rot = stim.rotation

  rot.z = rot.z + 0.01

  scale.x = 0.5
  scale.y = 0.5

  pos.x = 0.5
  pos.y = 0.5

  stim.position = pos
  stim.scale = scale
  stim.rotation = rot

  stim:Draw()
end

state.Render = {
  fixation = render_default
}

return state