Setup = {}

Setup.Entry = 'State1'

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
    height = 400,
    index = 0
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
    geometry = 'rect',
    texture = 'left/first',
    units = 'pixels',
    size = { 1.0, 1.0 },
    position = { 0.5, 0.5 },
    targets = { 'fixation' }
  },
  another = {
    geometry = 'rect'
  }
}

Setup.Targets = {
  fixation = {
    kind = 'Rectangle',
    padding = { 0.1, 0.1 },
    source = 'mouse'
  }
}

--  state 1

local State1 = {}

local function render_function_1()
  local eowu = eowu_script()
  local stim = eowu:Stimulus('fixation_square')
  stim:Draw()
  stim:Texture('2')

  local scale = stim.scale
  scale.x = scale.x + 1
  scale.y = scale.y + 1
  stim.scale = scale
end

State1.ID = '1'
State1.Duration = 1000
State1.Render = {
  hello = function() print('hi from lua') end,
  render1 = render_function_1
}

function State1.Entry()
  local eowu = eowu_script()
  local state = eowu:State('1')
  eowu:Render('render1')
  state:Next('1')
end

function State1.Exit()
  local eowu = eowu_script()
  local state = eowu:State('1')
  print(state:Ellapsed())
end

function State1.Loop()
  -- print('Looping!')
end

Setup.States = {
  State1 = State1
}