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

function state.Entry()
  local script = eowu_script()
  local state = script:State('images')
  local timing = state:Variable('state_time')

  state:Next('new_trial')
  script:Render('default')
  state:Variable('image_onset'):Reset()
  state:Variable('state_time'):Reset()
end

function state.Exit()
  local script = eowu_script()
  local state = script:State('images')
  local image = state:Variable('image_onset')
  local timing = state:Variable('state_time')

  image:Commit()

  timing:Set({
    exit = state:Ellapsed()
  })

  timing:Commit()
end

local function render_default()
  local script = eowu_script()
  local stim = script:Stimulus('sq')

  --  it's important to set the flip
  --  function within the render function
  script:Flip('default')

  stim:Units('normalized')
  stim:Texture('first')
  stim:Position({0.5, 0.5})

  local rot = stim.rotation
  local sz = stim.size

  stim:Rotation({0, 0, rot.z + 0.05})
  stim:Size({sz.x + 0.01, sz.y + 0.01})

  stim:Draw()
end

local function flip_default(id, time)
  if not id == 'main' then return end

  local script = eowu_script()
  local state = script:State('images')
  local image = state:Variable('image_onset')
  local val = image:Get()

  if not val.logged then
    print('logging')

    image:Set({
      time = time,
      logged = true,
      another = false,
      nested = {
        a = 'a', c = {'another one', 'another one'}
      },
    })
  end
end

state.Render = {
  default = render_default
}

state.Flip = {
  default = flip_default
}

return state