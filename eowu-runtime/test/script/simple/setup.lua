local fixation = require('states.fixation')

Setup = {}

Setup.Variables = {
  trial = 0
}

Setup.Windows = {
  main = {
    width = 400,
    height = 400
  }
}

Setup.Geometry = {
  Builtin = {
    rect = 'Rectangle'
  }
}

Setup.Stimuli = {
  fix = {
    geometry = 'rect',
    color = { 0, 1, 1 },
    size = { 0.5, 0.5 },
    position = { 0.5, 0.5 },
    units = 'normalized'
  }
}

Setup.States = { fixation }