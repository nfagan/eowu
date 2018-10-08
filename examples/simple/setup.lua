Setup = {}

Setup.Windows = {
  w1 = {
    resizeable = true
  }
}

Setup.Geometry = {
  --  'Circle', 'CircleFrame', 'Triangle', 'TriangleFrame'
  Builtin = {
    rect = 'Rectangle',
    rect_frame = 'RectangleFrame'
  }
}

Setup.Stimuli = {
  s1 = {}
}

Setup.States = {
  require('states.state1'),
  require('states.state2')
}