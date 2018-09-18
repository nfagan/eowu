local fixation = require('states.fixation')
local image = require('states.image')

Setup = {}

Setup.Windows = {
  main = {
    width = 600,
    height = 400,
    resizeable = false
  }
}

Setup.Sources = {
  position = {
    type = 'Mouse',
    window = 'main'
  }
}

Setup.Geometry = {
  Builtin = {
    rect = 'Rectangle',
    circ = 'Circle'
  }
}

Setup.Stimuli = {
  stim = {
    geometry = 'rect',
    units = 'mixed',
    size = { 100, 100 },
    position = { 1/3, 0.5 }
  },
  stim2 = {
    geometry = 'circ',
    units = 'mixed',
    size = { 200, 300 },
    color = { 1, 0, 0 },
    position = { 2/3, 0.5 }
  }
}

Setup.Targets = {
  first = {
    source = 'position',
    window = 'main',
    type = 'Circle',
    stimulus = 'stim',
    padding = { 50, 100 },
    hidden = false
  },
  sec = {
    source = 'position',
    window = 'main',
    type = 'Circle',
    stimulus = 'stim2',
    padding = { 0, 0 },
    hidden = false
  }
}

Setup.States = { fixation, image }
