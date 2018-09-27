local draw = require('states.draw')

Setup = {}

Setup.Variables = {
  trial = 0,
  frames = 0,
  delta = 0/0,
  props = {
    colors = {},
    positions = {}
  }
}

Setup.Windows = {
  main = {
    width = 400,
    height = 400,
    resizeable = true
  }
}

Setup.Geometry = {
  Builtin = {
    rect = 'Rectangle',
    circ = 'Circle',
    tri = 'Triangle'
  }
}

Setup.Stimuli = {}

local geoms = { 'rect', 'circ', 'tri' }

for i = 1, 10000 do
  local geom = ''

  local bin = (math.ceil(math.random() * 100) % 3) + 1
  local stim = { geometry = geoms[bin] }

  Setup.Stimuli['s' .. i] = stim
end

Setup.States = { draw }
