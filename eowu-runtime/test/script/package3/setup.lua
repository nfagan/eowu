local fixation = require('states.fixation')
local images = require('states.images')
local new_trial = require('states.new_trial')
local end_trial = require('states.end_trial')

Setup = {}

Setup.Variables = {
  trial = 0,
  frames = {
    images = 0/0,
    fixation = 0/0
  }
}

Setup.Paths = {
  Data = 'C:\\Users\\changLab\\Documents\\eowu\\'
}

Setup.Windows = {
  main = {
    width = 400, height = 400, fullscreen = false, resizeable = true
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
  },
  keyboard = {
    type = 'Keyboard'
  }
}

Setup.Textures = {}

Setup.Stimuli = {
  sq = { geometry = 'rect' },
  circ = { geometry = 'circ' }
}

for i = 1, 1000 do
  local stim = { geometry = 'circ' }
  Setup.Stimuli['sq' .. i] = stim
end

Setup.Targets = {}

Setup.States = { fixation, images, new_trial, end_trial }