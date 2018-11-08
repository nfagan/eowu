Setup = {}

Setup.Windows = {
  w1 = {
    width = 500,
    height = 500
  }
}

Setup.Geometry = {
  Builtin = {
    rect = 'Rectangle'
  }
}

Setup.Sources = {
  mouse = {
    type = 'Mouse',
    window = 'w1'
  }
}

local function make_target(stim)
  return {
    type = 'Circle',
    source = 'mouse',
    stimulus = stim,
    padding = {40, 40}
  }
end

local function make_stimulus(position, color)
  return {
    geometry = 'rect',
    units = 'mixed',
    position = position,
    color = color,
    size = {100, 100}
  }
end

Setup.Targets = {
  t1 = make_target('s1'),
  t2 = make_target('s2')
}

Setup.Stimuli = {
  s1 = make_stimulus({1/3, 1/2}, {1, 0, 0}),
  s2 = make_stimulus({2/3, 1/2}, {0, 1, 1}),
  s3 = make_stimulus({1/2, 1/2}, {0, 1, 0})
}

Setup.States = {
  require('states.choice'),
  require('states.choice_error'),
  require('states.choice_success')
}