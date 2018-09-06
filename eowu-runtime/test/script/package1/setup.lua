Setup = {}

Setup.Windows = {
  main = {
    width = 400, height = 400
  }
}

Setup.Geometry = {
  Builtin = {
    [1] = 'Rectangle'
  }
}

Setup.Sources = {}

Setup.Textures = {}

Setup.Stimuli = {
  sq = {
    geometry = '1'
  }
}

Setup.Targets = {}

Setup.States = {
  State1 = {
    ID = '1',
    Duration = -1,
    Entry = function() end,
    Loop = function() end,
    Exit = function() end,
    Render = {
      hello = (function()
        local stim = eowu_script():Stimulus('sq')
        stim:Units('normalized')
        stim:Color(1, 1, 1)
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
}