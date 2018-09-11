local state = {}

state.Name = 'fixation'
state.Duration = 1000
state.Variables = {}

function state.Entry()
  local script = eowu_script()
  local state = script:State('fixation')
  state:Next('images')
  script:Render('fixation', 'fixation')
end

function state.Exit()
  --
end

local function flip(a, b)

end

local function render()
  local script = eowu_script()
  local stim = script:Stimulus('sq')
  local renderer = script:Renderer()

  stim:Units('normalized')
  stim:Color({1, 0, 0})
  stim:Position({0.5, 0.5})

  local rot = stim.rotation
  local sz = stim.size

  stim:Rotation({0, 0, rot.z - 0.05})
  stim:Size({sz.x - 0.01, sz.y - 0.01})

  stim:Draw()
end

state.Render = {
  fixation = render
}

state.Flip = {
  fixation = flip
}

return state