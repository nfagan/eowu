local first_state = require('states.first')

Setup = {}

Setup.Windows = {
  main = {
    width = 400,
    height = 400,
    resizeable = true
  }
}

Setup.Geometry = {
  Builtin = {
    rect = 'Rectangle',
    circ = 'Circle'
  }
}

Setup.Stimuli = {
  s1 = {
    geometry = 'circ',
    position = { 0.5, 0.5 },
    units = 'normalized'
  }
}

Setup.Sources = {
  mouse = {
    type = 'Mouse',
    window = 'main'
  }
}

Setup.Targets = {
  t1 = {
    type = 'Rectangle',
    stimulus = 's1',
    source = 'mouse',
    window = 'main'
  }
}

Setup.States = { first_state }