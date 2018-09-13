local state = {}

state.Name = 'fixation'
state.Duration = 200
state.Variables = {
  frames = 0
}

function state.Entry()
  local script = eowu_script()
  local state = script:State('fixation')
  state:Next('images')
  script:Render('fixation', 'fixation')
  local frames = state:Variable('frames')
  frames:Reset()

  local task_frames = script:Variable('frames')

  task_frames:Set({
    images = script:State('images'):Variable('frames'):Get(),
    fixation = task_frames:Get().fixation
  })
end

local function flip(id, time)

end

local function render()
  local script = eowu_script()
  local stim = script:Stimulus('sq')
  local renderer = script:Renderer()
  local state = script:State('fixation')
  local frames = state:Variable('frames')

  frames:Set(frames:Get()+1)

  stim:Units('normalized')
  stim:Color({0, 0, 0})
  stim:Position({0.5, 0.5})

  local rot = stim.rotation
  local sz = stim.size

  stim:Rotation({0, 0, rot.z - 0.05})
  stim:Size({sz.x + 0.01, sz.y + 0.01})

  for i = 1, 100 do
    local s = script:Stimulus('sq' .. i)
    s:Units('normalized')
    s:Position({math.random(), math.random()})
    s:Size({0.01, 0.01})
    s:Color({0, 1, 0})
    s:Draw()
  end

  stim:Draw()
end

state.Render = {
  fixation = render
}

state.Flip = {
  fixation = flip
}

return state