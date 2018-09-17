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
  example = {
    geometry = 'rect'
  }
}

Setup.States = { fixation }