local state1 = require('states.state1')

Setup = {}

Setup.Windows = {
  main = {
    width = 400,
    height = 400
  }
}

Setup.Geometry = {
  Builtin = {
    rect = 'Rectangle',
    circ = 'Circle',
    tri = 'Triangle'
  }
}

Setup.Stimuli = {
  s1 = { geometry = 'rect' },
  s2 = { geometry = 'circ' },
  s3 = { geometry = 'tri' }
}

Setup.Textures = {
  t1 = '/Users/Nick/Desktop/eg.png'
}

Setup.States = { state1 }