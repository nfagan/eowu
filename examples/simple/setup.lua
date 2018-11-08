Setup = {}

Setup.Windows = {
  w1 = {
    resizeable = true
  }
}

Setup.Geometry = {
  Builtin = {
    rect = 'Rectangle'
  }
}

Setup.Stimuli = {
  s1 = {
    geometry = 'rect'
  }
}

Setup.States = {
  require('states.state1')
}