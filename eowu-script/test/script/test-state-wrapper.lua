State1 = {}
State2 = {}

State1.ID = '1'
State2.ID = '2'

State1.Duration = 1000
State2.Duration = 1000

local function state1_render()
  local stim = eowu.GL.Stimulus('first')
  -- Position
  local pos = stim.position
  pos.x = 0.333
  pos.y = 0.5
  stim.position = pos
  -- Color
  stim:Color(1, 1, 1)
  stim:Units('normalized')
  stim:Draw()

end

local function state2_render()
  local stim = eowu.GL.Stimulus('first')
  -- State
  local state = eowu.Task.State('1')
  -- Position
  local pos = stim.position
  pos.x = 0.666
  pos.y = 0.5
  stim.position = pos
  -- Color
  stim:Color(1, 1, 0)
  stim:Units('normalized')
  stim:Draw({1})
end

function State1.Entry()
  print('State1!')
  local s = eowu.Task.State('1')
  s:Next('2')

  eowu.GL.Renderer():Render(state1_render)
end

function State1.Exit()
  local s = eowu.Task.State('1')
  print(s:Ellapsed())
end

function State1.Loop()  
end

function State1.Render()
  -- local model = eowu.
end

function State2.Entry()
  print('State2!')
  local s = eowu.Task.State('2')
  s:Next('1')
  eowu.GL.Renderer():Render(state2_render)
end

function State2.Exit()
end

function State2.Loop()
end

function State2.Render()
end

States = {
  State1 = State1,
  State2 = State2
}