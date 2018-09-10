local fixation = require('states.fixation')
local images = require('states.images')
local new_trial = require('states.new_trial')

Setup = {}

Setup.Paths = {
  Data = '/Users/Nick/Desktop/test/',
}

Setup.Windows = {
  main = {
    width = 300, height = 300, fullscreen = false, index = 0
  }
}

Setup.Geometry = {
  Builtin = {
    rect = 'Rectangle',
    circ = 'Circle'
  }
}

Setup.Sources = {
  mouse = {
    type = 'Mouse'
  }
}

Setup.Textures = {
  first = '/Users/Nick/Desktop/eg.png'
}

Setup.Stimuli = {
  sq = { geometry = 'rect' },
  circ = { geometry = 'circ' }
}

for i = 1, 1000 do
  local stim = { geometry = 'circ' }
  Setup.Stimuli['sq' .. i] = stim
end

Setup.Targets = {}

Setup.States = { fixation, images, new_trial }