local fixation = require('states.fixation')
local image = require('states.image')

Setup = {}

Setup.Windows = {
  main = {
    width = 800,
    height = 400,
    resizeable = true
  }
}

Setup.Lifecycle = {
  Startup = function() end,
  Shutdown = function() end
}

Setup.Sources = {
  position = {
    type = 'Mouse',
    window = 'main'
  }
}

Setup.Geometry = {
  Builtin = {
    rect = 'Rectangle',
    circ = 'Circle'
  }
}

Setup.Stimuli = {
  stim = {
    geometry = 'rect',
    units = 'mixed',
    size = { 100, 100 },
    position = { 1/3, 0.5 }
  },
  stim2 = {
    geometry = 'circ',
    units = 'mixed',
    size = { 200, 100 },
    color = { 1, 0, 0 },
    position = { 2/3, 0.5 }
  }
}

Setup.Targets = {
  t1 = {
    source = 'position',
    window = 'main',
    type = 'Circle',
    stimulus = 'stim',
    padding = { 50, 50 },
    hidden = false
  },
  t2 = {
    source = 'position',
    window = 'main',
    type = 'Circle',
    stimulus = 'stim2',
    padding = { 0, 0 },
    hidden = false
  }
}

Setup.States = { fixation, image }
-- Setup.Textures = {}

-- local function split(str, sep)
--   local result = {}
--   local regex = ("([^%s]+)"):format(sep)
--   for each in str:gmatch(regex) do
--      table.insert(result, each)
--   end
--   return result
-- end

-- local p = '/Volumes/My Passport/NICK/Chang Lab 2016/hwwba/stimuli/sm/cued-image/nonsocial/'
-- local escaped_p = '"' .. p .. '"'
-- local ls = io.popen('ls ' .. escaped_p)
-- local res = ls:read('*a')
-- ls:close()

-- local files = split(res, '\n')

-- for i = 1, #files do
--   Setup.Textures['first' .. i] = p .. files[i]
-- end