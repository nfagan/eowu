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
local start_offset = 100
local mean_iter = 1
local N = 500
local frame_times = {}

local function mean(times)
  local sum = 0
  for i = 1, #times do
    sum = sum + times[i]
  end
  return sum / #times
end

local function std(times)
  local mn = mean(times)
  local sum = 0
  for i = 1, #times do
    local v = times[i] - mn
    sum = sum + (v*v)
  end

  local mean_sum = sum / (#times-1)
  return math.sqrt(mean_sum)
end

local function render_default()
  local renderer = eowu_script():Renderer()
  local delta = renderer:Delta() * 1000

  local state = eowu_script():State('fixation')

  if idx > start_offset then
    frame_times[mean_iter] = delta
    mean_iter = mean_iter + 1
  end

  idx = idx + 1

  if mean_iter > N then
    print('MEAN: ', mean(frame_times))
    print('DEV: ', std(frame_times))
    state:Exit()
  end

  local stim = eowu_script():Stimulus('sq')
  stim:Units('mixed')
  stim:Color({1, 0, 0})
  stim:Position({0.5, 0.5})
  stim:Scale({100, 100})

  local rot = stim.rotation

  stim:Rotation({0, 0, rot.z + 0.001})

  stim:Draw({'main'})

  local eowu = eowu_script()

  for i = 1, 100 do
    local stim = eowu:Stimulus('sq' .. i)
    stim:Units('mixed')
    stim:Position({math.random(), math.random()})
    stim:Scale({10, 10})
    stim:Texture('first')
    stim:Draw()
  end
end

state.Render = {
  fixation = render_default
}

return state