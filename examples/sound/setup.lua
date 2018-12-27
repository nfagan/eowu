Setup = {}

local join = eowu.fs.join
local res_root = join({eowu.fs.root(), 'res'})

Setup.Windows = {
  w1 = {
    resizeable = true,
  }
}

Setup.Geometry = {
  Builtin = {
    circ = 'Circle'
  }
}

Setup.Fonts = {
  cour = join({res_root, 'fonts', 'cour.ttf'})
}

Setup.Sounds = {
  guitar = join({res_root, 'sounds', 'guitar.wav'}),
  piano = join({res_root, 'sounds', 'piano.wav'})
}

Setup.Textures = {
  t1 = join({res_root, 'textures', 'eg.png'})
}

Setup.States = {
  require('states.state1')
}