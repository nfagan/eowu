local fixation = require('states.fixation')
local images = require('states.images')

Setup = {}

Setup.Windows = {
  main = {
    width = 400, height = 400
  },
  secondary = {
    width = 400, height = 400
  }
}

Setup.Geometry = {
  Builtin = {
    rect = 'Rectangle',
    circ = 'Circle'
  }
}

Setup.Sources = {}

Setup.Textures = {
  first = '/Users/Nick/Desktop/eg.png'
}

Setup.Stimuli = {
  sq = { geometry = 'rect' },
  circ = { geometry = 'rect' }
}

Setup.Targets = {}

Setup.States = { fixation, images }