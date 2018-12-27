local function make_state()
  local state = {}

  state.Name = 'state1'
  state.Duration = 1000   --  (ms) Use -1 for infinite.
  state.First = true      --  Begin in this state.
  state.Variables = {     --  Variables are accessible across render and task contexts.
    trial = 0
  }

  --  entry function: Called once upon entering the state.
  function state.Entry()
    local state = eowu.CurrentState()
    --  Begin calling the render function bound to 'default'
    eowu.Render('default')

    --  When the time limit for this state is met, proceed to the state given by 'state1'
    --  If no state is marked with Next() by the time the current state exits, the program will exit.
    state:Next('state1')

    print('Entered: ' .. state.name)
  end

  --  exit function: Called once upon exiting the state.
  function state.Exit()
    local state = eowu.CurrentState()
    local trial = state:Variable('trial')

    trial:Set(trial:Get() + 1)

    local elapsed_str = 'Elapsed: ' .. state:Elapsed() * 1e3 .. ' ms'
    print('Exited: ' .. state.name .. '; ' .. elapsed_str)
  end

  --  render function: Called each render frame.
  local function render_default()
    local s1 = eowu.Stimulus('s1')
    local s2 = eowu.Stimulus('s2')
    local kb = eowu.Keyboard()
    local color = {0, 0, 1}
    local geometry = 'rect'

    local trial = eowu.State('state1'):Variable('trial')

    if trial:Get() % 2 == 0 then
      if kb:Down('space') then
        s1:ZRotate(-0.05)
        color = {1, 0, 0}
      else
        s1:ZRotate(0.01)
        color = {0, 0, 1}
      end
    else
      geometry = 'frame'
    end

    --  Units can be 'mixed', 'normalized', or 'pixels'.
    --    'normalized': Position and Size are normalized to the screen dimensions.
    --    'pixels': Position and Size are in pixels.
    --    'mixed': Position is normalized to the screen dimensions, but Size is in pixels.
    s1:Units('mixed')
    s1:Position({0.5, 0.5})
    s1:Size({200, 200})
    s1:Color(color)
    s1:Geometry(geometry)

    s2:Units('mixed')
    s2:Position({0.5, 0.5})
    s2:Size({50, 50})
    s2:Geometry('rect')
    s2:ZRotate(0.01)
    s2:Color({1})
    
    s1:Draw()
    s2:Draw()
  end

  state.Render = {
    default = render_default
  }

  return state
end

--  Setup is a global table that must exist when `eowu-run` is executed.
Setup = {}

--  Windows can only be opened by specifying them in the Setup table.
Setup.Windows = {
  w1 = {}
}

Setup.Geometry = {
  Builtin = {
    rect = 'Rectangle',  -- Bind builtin Rectangle geometry to the id 'rect'
    frame = 'RectangleFrame'
  }
}

Setup.Stimuli = {
  s1 = {},
  s2 = {}
}

Setup.States = { make_state() }