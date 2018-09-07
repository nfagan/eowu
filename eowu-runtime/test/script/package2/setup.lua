local fixation = require('states.fixation')
local images = require('states.images')

Setup = {}

Setup.Windows = {
  main = {
    width = 400, height = 400, fullscreen = true, index = 0
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
  sq = { geometry = 'circ' },
  circ = { geometry = 'circ' }
}

for i = 1, 1000 do
  local stim = { geometry = 'circ' }
  Setup.Stimuli['sq' .. i] = stim
end

Setup.Targets = {}

Setup.States = { fixation, images }