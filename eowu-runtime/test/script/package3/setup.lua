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
  data = 'C:\\Users\\changLab\\Repositories\\cpp\\eowu\\data\\'
  -- data = '/Users/Nick/repositories/cpp/eowu/data/'
}

Setup.Save = {
  states = true
}

Setup.Windows = {
  main = {
    width = 400, height = 400, fullscreen = false, resizeable = true
  }
}

Setup.Sources = {
  position = {
    type = 'Mouse',
    window = 'main'
  },
  keyboard = {
    type = 'Keyboard'
  }
}

Setup.Targets = {
  first = {
    source = 'position',
    window = 'main',
    type = 'Circle',
    padding = { 0, 0 },
    stimulus = 'sq',
  }
}

Setup.Textures = {}

Setup.Geometry = {
  Builtin = {
    rect = 'Rectangle',
    circ = 'Circle'
  }
}

Setup.Stimuli = {
  sq = { geometry = 'rect' },
  circ = { geometry = 'circ', targets = {'first'} }
}

for i = 1, 1000 do
  local stim = { geometry = 'circ' }
  Setup.Stimuli['sq' .. i] = stim
end

Setup.States = { fixation, images, new_trial, end_trial }