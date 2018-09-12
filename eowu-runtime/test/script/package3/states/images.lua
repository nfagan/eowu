local state = {}

state.Name = 'images'
state.Duration = 1000

state.Variables = {
  image_onset = {
    time = 0/0,
    logged = false,
    another = false,
  },
  state_time = {
    entry = 0/0,
    exit = 0/0
  }
}

local printed_time = false
local last_t = nil

function state.Entry()
  local script = eowu_script()
  local state = script:State('images')
  local timing = state:Variable('state_time')
  local first = script:Variable('trial')

  state:Next('end_trial')
  last_t = state:Ellapsed()
  printed_time = false
  script:Render('image', 'image')

  state:Variable('image_onset'):Reset()
  state:Variable('state_time'):Reset()
end

function state.Loop()
  if not printed_time then
    local script = eowu_script()
    local state = script:State('images')
    print('Time waiting to render: ', (state:Ellapsed() - last_t)*1000, 'ms')
    printed_time = true
    last_t = state:Ellapsed()
  end
end

function state.Exit()
  local script = eowu_script()
  local state = script:State('images')
  local image = state:Variable('image_onset')
  local timing = state:Variable('state_time')

  while not image:Get().logged do end

  image:Commit()

  timing:Set({
    exit = state:Ellapsed()
  })

  timing:Commit()
end

--  render functions

local function render()
  local script = eowu_script()
  local stim = script:Stimulus('sq')

  stim:Units('normalized')
  stim:Color({1, 1, 1})

  local rot = stim.rotation
  local sz = stim.size

  stim:Rotation({0, 0, rot.z + 0.05})
  stim:Size({sz.x + 0.01, sz.y + 0.01})

  stim:Draw()
end

--  flip functions: called after render function

local function flip(id, time)
  if id ~= 'main' then return end

  local script = eowu_script()
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