Setup = {}

Setup.Windows = {
  main = {
    width = 400, height = 400
  }
}

Setup.Geometry = {
  Builtin = {
    rect = 'Rectangle',
    circ = 'Circle'
  }
}

Setup.Sources = {}

Setup.Textures = {
  first = '/Users/Nick/Desktop/eg.png'
}

Setup.Stimuli = {
  sq = { geometry = 'rect' },
  circ = { geometry = 'rect' }
}

Setup.Targets = {}

local State1 = {
  ID = '1',
  Duration = 1000,
  First = false,
  Entry = (function()
    print('entered1')
    eowu_script():Render('hello')
    eowu_script():State('1'):Next('2')
  end),
  Loop = function() end,
  Exit = function() end,
  Render = {
    hello = (function()
      local stim = eowu_script():Stimulus('sq')
      stim:Units('normalized')
      -- stim:Color(1, 1, 1)
      stim:Texture('first')
      local scale = stim.scale
      local pos = stim.position
      pos.x = 0.5
      pos.y = 0.5
      scale.x = 0.5
      scale.y = 0.5
      stim.scale = scale
      stim.position = pos
      stim:Draw()
    end)
  }
}

local State2 = {
  ID = '2',
  Duration = 1000,
  Entry = (function()
    eowu_script():Render('hello2')
    eowu_script():State('2'):Next('1')
  end),
  Loop = function() end,
  Exit = (function() 
    print(eowu_script():State('2'):Ellapsed() * 1000)
  end),
  Render = {
    hello2 = (function()
      local stim = eowu_script():Stimulus('sq')
      stim:Units('normalized')
      stim:Color(1, 1, 0.5)
      -- stim:Texture('first')
      local scale = stim.scale
      local pos = stim.position
      pos.x = 0.5
      pos.y = 0.5
      scale.x = 0.3333
      scale.y = 0.5
      stim.scale = scale
      stim.position = pos
      stim:Draw()
    end)
  }
}

Setup.States = { State1, State2 }