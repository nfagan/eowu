local timeout = require('states.timeout')

Setup = {}

Setup.Windows = {
  main = {
    width = 400,
    height = 400
  }
}

Setup.Sources = {
  mouse = {
    type = 'Mouse',
    window = 'main'
  }
}

Setup.Geometry = {
  Builtin = {
    rect = 'Rectangle'
  }
}

Setup.Targets = {
  t1 = {
    window = 'main',
    type = 'Circle',
    source = 'mouse',
    stimulus = 's1',
    padding = { 0.1, 0.1 }
  }
}

Setup.Stimuli = {
  s1 = {
    geometry = 'rect'
  }
}

Setup.States = { timeout }