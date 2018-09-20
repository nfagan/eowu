local draw = {}

local function print_frame_info()
  local script = eowu.script()
  local state = script:State('draw')
  local trial = script:Variable('trial')
  local frames = script:Variable('frames')
  local delta = script:Variable('delta')

  local tn = trial:Get()
  local f = frames:Get()
  local t = state:Ellapsed()
  local d = delta:Get() * 1000

  local trial_info = 'Completed trial: ' .. tn
  local time_info = '; time: ' .. t .. ' (s)'
  local frame_info = '; frames: ' .. f
  local delta_info = '; delta: ' .. d .. ' (ms)'

  print(trial_info .. time_info .. frame_info .. delta_info)
end

local function reset_frame_info()
  local script = eowu.script()
  local trial = script:Variable('trial')
  local frames = script:Variable('frames')
  local delta = script:Variable('delta')
  local tn = trial:Get()

  trial:Set(tn + 1)
  frames:Reset()
  delta:Reset()
end

local function entry()
  local script = eowu.script()
  script:Render('draw')
end

--  render function: Called each render-frame.

local first_frame = true

local function render()
  local script = eowu.script()
  local renderer = script:Renderer()

  local tn = script:Variable('trial'):Get()
  local frames = script:Variable('frames')
  local delta = script:Variable('delta')
  local n_frames = frames:Get()

  if n_frames == 120 then
    print_frame_info()
    reset_frame_info()
  end

  local d = renderer:Delta()

  if not first_frame then
    local current_delta = delta:Get()

    if eowu.math.isnan(current_delta) then
      delta:Set(d)
    else
      local weighted_delta = current_delta * n_frames
      local new_delta = (weighted_delta + d) / (n_frames + 1)
      delta:Set(new_delta)
    end
  end

  first_frame = false

  for i = 1, 100 do
    local stim = script:Stimulus('s' .. i)
    stim:Units('normalized')
    stim:Position({math.random(), math.random()})
    stim:Size({0.05, 0.05})
    stim:Color({math.random(), math.random(), math.random()})
    stim:Draw()
  end

  frames:Set(frames:Get() + 1)
end

draw.First = true
draw.Name = 'draw'
draw.Duration = -1
draw.Entry = entry
draw.Render = {
  draw = render         
}

return draw