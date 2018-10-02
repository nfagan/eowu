local state1 = require('states.state1')

Setup = {}

Setup.Windows = {
  main = {
    width = 400,
    height = 600,
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
  s1 = { geometry = 'rect' },
  s2 = { geometry = 'circ' },
  s3 = { geometry = 'rect' }
}

Setup.Textures = {
  t1 = eowu.fs.fullfile({eowu.fs.root(), 'res/textures/eg.png'})
}

Setup.Sources = {
  mouse = { 
    type = 'Mouse',
    window = 'main'
  }
}

Setup.Targets = {
  t1 = {
    stimulus = 's1',
    source = 'mouse',
    window = 'main',
    type = 'Circle'
  }
}

Setup.States = { state1 }