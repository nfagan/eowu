local state = {}

state.Name = 'state1'
--  Duration: ms
state.Duration = 1000
--  Begin in this state
state.First = true

--  entry function: Called once upon entering the state.
function state.Entry()

  --  eowu.script() is the global object through which other resources are accessed.
  local script = eowu.script()
  local state = script:State('state1')

  --  Set the active render function to the function given by id 'state1'. This function
  --  will be called beginning on the next available render frame.
  script:Render('state1')

  --  When the time limit for this state is met, proceed to the state given by 'state2'
  --  If no state is marked with Next() by the time the current state exits, the task will exit.
  state:Next('state2')

  print('Entered: ' .. state.name)
end

--  exit function: Called once upon exiting the state.
function state.Exit()
  local state = eowu.script():State('state1')

  print('Exited: ' .. state.name)
end

--  render function: Called each render frame. Below,
--  state.Render{} maps this function to the id 'state1'.
local function state1_render()
  local script = eowu.script()
  local s1 = script:Stimulus('s1')
  local kb = script:Keyboard()
  local color
  local geom

  if kb:Down('space') then
    s1:ZRotate(-0.05)
    color = {1, 0, 0}
    geom = 'rect_frame'
  else
    s1:ZRotate(0.01)
    color = {0, 0, 1}
    geom = 'rect'
  end

  --  Units can be 'mixed', 'normalized', or 'pixels'.
  --    'normalized': Position and Size are normalized to the screen dimensions.
  --    'pixels': Position and Size are in pixels.
  --    'mixed': Position is normalized to the screen dimensions, but Size is in pixels.
  s1:Units('mixed')
  s1:Position({0.5, 0.5})
  s1:Size({200, 200})
  s1:Color(color)
  s1:Geometry(geom)

  s1:Draw()
end

state.Render = {
  state1 = state1_render
}

return state