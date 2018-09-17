local state = {}

state.Name = 'images'
state.Duration = 200

state.Variables = {
  image_onset = {
    time = 0/0,
    logged = false,
    another = false,
  },
  render_time = 0/0,
  state_time = {
    entry = 0/0,
    exit = 0/0
  },
  frames = 0
}

local printed_time = false
local last_t = nil

function state.Entry()
  local script = eowu.script()
  local state = script:State('images')
  local timing = state:Variable('state_time')
  local first = script:Variable('trial')

  state:Next('end_trial')
  last_t = state:Ellapsed()
  printed_time = false
  script:Render('image', 'image')

  state:Variable('image_onset'):Reset()
  state:Variable('state_time'):Reset()
  state:Variable('frames'):Reset()
  state:Variable('render_time'):Reset()

  local task_frames = script:Variable('frames')
  
  task_frames:Set({
    images = task_frames:Get().images,
    fixation = script:State('fixation'):Variable('frames'):Get()
  })
end

function state.Loop()
  if not printed_time then
    local script = eowu.script()
    local state = script:State('images')
    state:Variable('render_time'):Set(state:Ellapsed() - last_t)
    state:Variable('render_time'):Commit()
    printed_time = true
    last_t = state:Ellapsed()
  end

  local script = eowu.script()
end

function state.Exit()
  local script = eowu.script()
  local state = script:State('images')
  local image = state:Variable('image_onset')
  local timing = state:Variable('state_time')
  local frames = state:Variable('frames')

  -- while not image:Get().logged do end

  image:Commit()

  timing:Set({
    exit = state:Ellapsed()
  })

  timing:Commit()
end

local function clamp(a, b, c)
  if b ~= nil and a < b then return b end
  if c ~= nil and a > c then return c end
  return a
end

--  render functions

local function render()
  local script = eowu.script()
  local state = script:State('images')
  local frames = state:Variable('frames')
  local stim = script:Stimulus('sq')

  for i = 1, 100 do
    local s = script:Stimulus('sq' .. i)
    s:Units('normalized')
    s:Position({math.random(), math.random()})
    s:Size({0.01, 0.01})
    s:Color({1, 0, 0})
    s:Draw()
  end

  stim:Units('normalized')
  stim:Color({1, 1, 1})

  local rot = stim.rotation
  local sz = stim.size

  sz.x = clamp(sz.x - 0.01, 0, nil)
  sz.y = clamp(sz.y - 0.01, 0, nil)

  stim:Rotation({0, 0, rot.z + 0.05})
  stim.size = sz

  stim:Draw()

  frames:Set(frames:Get()+1)
end

--  flip functions: called after render function

local function flip(id, time)
  if id ~= 'main' then return end

  local script = eowu.script()
  local state = script:State('images')
  local image = state:Variable('image_onset')
  local val = image:Get()

  if val.logged then return end

  image:Set({
    time = time,
    logged = true,
    another = false,
    nested = {
      a = 'a', c = {'another one', 'another one'}
    },
  })
end

state.Render = {
  image = render
}

state.Flip = {
  image = flip
}

return state