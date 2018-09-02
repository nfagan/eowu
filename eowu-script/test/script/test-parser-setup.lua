Setup = {}

Setup.Sources = {
  eyelink = {
    type = 'Eyelink',
    values = { 'Position' }
  },
  mouse = {
    type = 'Mouse',
    values = { 'Position' }
  },
  keyboard = { 
    type = 'Keyboard' 
  },
  reward = {
    type = 'Reward',
    port = 'COM3',
    channels = { 1, 2 }
  }
}

Setup.Windows = {
  main = {
    width = 400,
    height = 400
  }
}

Setup.Geometry = {
  Builtin = {
    rect = 'Rectangle',
    circ = 'Circle'
  }
}

Setup.Textures = {
  ['left/first'] = '/Users/Nick/Desktop/eg.png'
}

Setup.Stimuli = {
  fixation_square = {
    geometry = 'circ',
    texture = 'left/first',
    units = 'normalized',
    size = { 1.0, 0.5 },
    position = { 0.5, 0.5 },
    targets = { 'fixation', 'another one' }
  },
}

Setup.Targets = {
  fixation = {
    kind = 'Rectangle',
    padding = { 0.1, 0.1 },
    source = 'mouse'
  }
}