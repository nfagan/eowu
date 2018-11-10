Setup = {}

local fullfile = eowu.fs.fullfile

local res_root = fullfile({eowu.fs.root(), 'res'})
local sound_root = fullfile({res_root, 'sounds'})
local texture_root = fullfile({res_root, 'textures'})

Setup.Windows = {
  w1 = {
    resizeable = true,
    fullscreen = false
  }
}

Setup.Geometry = {
  Builtin = {
    circ = 'Circle'
  }
}

Setup.Sounds = {
  guitar = fullfile({sound_root, 'guitar.wav'}),
  piano = fullfile({sound_root, 'piano.wav'})
}

Setup.Stimuli = {
  s1 = {}
}

Setup.Textures = {
  t1 = fullfile({texture_root, 'eg.png'}),
  t2 = fullfile({texture_root, 'eg2.png'})
}

Setup.States = {
  require('states.state1')
}