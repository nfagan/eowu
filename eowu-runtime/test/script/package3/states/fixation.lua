local state = {}

local NAME = 'fixation'

state.Name = NAME
state.Duration = 1000
state.Variables = {}

function state.Entry()
  local script = eowu_script()
  local state = script:State(NAME)
  state:Next('images')
  script:Render(NAME)
end

function state.Exit()
  print(eowu_script():State(NAME):Ellapsed()*1000)
end

local function render_default()
  local script = eowu_script()
  local stim = script:Stimulus('sq')

  stim:Units('normalized')
  stim:Color({1, 0, 0})
  stim:Position({0.5, 0.5})

  local rot = stim.rotation
  local sz = stim.size

  stim:Rotation({0, 0, rot.z + 0.05})
  stim:Size({sz.x - 0.01, sz.y - 0.01})

  stim:Draw()
end

state.Render = {
  fixation = render_default
}

state.Flip = {
  fixation = render_default
}

return state