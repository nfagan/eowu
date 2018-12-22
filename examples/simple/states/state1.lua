local state = {}

state.Name = 'state1'
state.Duration = 1000   --  (ms)
state.First = true      --  Begin in this state
state.Variables = {     --  Variables are accessible across render and task contexts.
  trial = 0
}

--  entry function: Called once upon entering the state.
function state.Entry()

  local state = eowu.State('state1')
  local trial = state:Variable('trial')
  local trial_number = trial:Get()

  --  Set the active render function. This function will be called beginning 
  --  on the next available render frame.
  if trial_number % 2 == 0 then
    eowu.Render('even')
  else
    eowu.Render('odd')
  end

  --  When the time limit for this state is met, proceed to the state given by 'state2'
  --  If no state is marked with Next() by the time the current state exits, the task will exit.
  state:Next('state1')

  print('Entered: ' .. state.name)
end

--  exit function: Called once upon exiting the state.
function state.Exit()
  local state = eowu.CurrentState()
  local trial = state:Variable('trial')

  trial:Set(trial:Get() + 1)
end

--  render function: Called each render frame. Below,
--  state.Render{} maps this function to the id 'even'.
local function render_even()
  local s1 = eowu.Stimulus('s1')
  local kb = eowu.Keyboard()
  local color

  if kb:Down('space') then
    s1:ZRotate(-0.05)
    color = {1, 0, 0}
  else
    s1:ZRotate(0.01)
    color = {0, 0, 1}
  end

  --  Units can be 'mixed', 'normalized', or 'pixels'.
  --    'normalized': Position and Size are normalized to the screen dimensions.
  --    'pixels': Position and Size are in pixels.
  --    'mixed': Position is normalized to the screen dimensions, but Size is in pixels.
  s1:Units('mixed')
  s1:Position({0.5, 0.5})
  s1:Size({200, 200})
  s1:Color(color)
  s1:Geometry('rect')

  s1:Draw()
end

--  Another render function. This one's called every odd-numbered trial.

local function render_odd()
  local kb = eowu.Keyboard()
  local s1 = eowu.Stimulus('s1')

  s1:Color({1, 1, 1})

  if kb:Down('space') then
    s1:ZRotate(-0.05)
  else
    s1:ZRotate(0.01)
  end

  s1:Draw()
end

state.Render = {
  even = render_even,
  odd = render_odd
}

return state