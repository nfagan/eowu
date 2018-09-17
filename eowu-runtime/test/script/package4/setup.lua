local fixation = require('states.fixation')
local images = require('states.images')

Setup = {}

Setup.Variables = {}

Setup.Paths = {
  -- data = 'C:\\Users\\changLab\\Documents\\eowu\\'
  data = '/Users/Nick/repositories/cpp/eowu/data/'
}

Setup.Save = {
  states = false,
}

Setup.Windows = {
  main = {
    width = 400,
    height = 400
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
    type = 'Rectangle',
    padding = { 0, 0 },
    stimulus = 'sq',
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
  sq = { 
    geometry = 'rect', 
    color = { 1, 0, 1 },
    size = { 0.5, 0.5 },
    position = { 0.5, 0.5 },
    units = 'normalized'
  },
}

for i = 1, 1000 do
  local stim = { geometry = 'circ' }
  Setup.Stimuli['sq' .. i] = stim
end

Setup.States = { fixation, images }