State1 = {}
State2 = {}
State3 = {}

State1.ID = '1'
State2.ID = '2'
State3.ID = '3'

State1.Duration = 1000
State2.Duration = 1000
State3.Duration = 1000

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

local last_time = nil
local max_time = nil

local function state2_render()
  local stim = eowu.GL.Stimulus('first')
  -- State
  local state = eowu.Task.State('2')
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

local function state3_render()
  local stim = eowu.GL.Stimulus('first')
  local state = eowu.Task.State('3')
  stim:Color(0, 0, 1)

  -- stim:Color(0.5, 0.5, 0.5)

  local pos = stim.position
  pos.x = pos.x + 0.001
  pos.y = pos.y + 0.001

  stim.position = pos

  stim:Draw()

  for i = 1, 1000 do
    local stim = eowu.GL.Stimulus('first' .. i)
    stim:Units('normalized')
    stim:Color(1, 1, 1)
    local pos = stim.position
    local scale = stim.scale
    scale.x = 0.01
    scale.y = 0.01
    pos.x = math.random()
    pos.y = math.random()
    stim.position = pos
    stim.scale = scale
    stim:Draw()
  end
end

function State1.Entry()
  -- print('State1!')
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

function State2.Entry()
  local s = eowu.Task.State('2')
  s:Next('3')
  eowu.GL.Renderer():Render(state2_render)
end

function State2.Exit()
end

function State2.Loop()
end

function State3.Entry()
  local a = eowu.Task.State('3')
  a:Next('1')
  eowu.GL.Renderer():Render(state3_render)
end

function State3.Exit()
end

function State3.Loop()
end

States = {
  State1 = State1,
  State2 = State2,
  State3 = State3
}