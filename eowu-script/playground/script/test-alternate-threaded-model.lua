State = {}
State2 = {}
State3 = {}

State.ID = '1'
State.Duration = 1000

State2.ID = '2'
State2.Duration = 1000

State3.ID = '3'
State3.Duration = 1000

function State2.Entry()
  local eowu = eowu_script()
  local state = eowu:State('2')
  eowu:Render('state2')
  state:Next('3')
end

function State2.Exit() end
function State2.Loop() end

function State3.Entry()
  local eowu = eowu_script()
  local state = eowu:State('3')
  eowu:Render('state3')
  state:Next('1')
end

function State3.Loop() end
function State3.Exit() end

function State.Entry()
  local eowu = eowu_script()
  local state = eowu:State('1')

  eowu:Render('state1')
  state:Next('2')
end

function State.Exit()
  local eowu = eowu_script()
  local state = eowu:State('1')
  local ellapsed = state:Ellapsed()
end

function State.Loop() end

local function state1_render()
  local eowu = eowu_script()
  local stim = eowu:Stimulus('first')
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
  local eowu = eowu_script()
  local stim = eowu:Stimulus('first')
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
  local eowu = eowu_script()
  local stim = eowu:Stimulus('first')
  stim:Color(0, 0, 1)

  local pos = stim.position
  pos.x = pos.x + 0.001
  pos.y = pos.y + 0.001

  stim.position = pos

  stim:Draw()

  for i = 1, 1000 do
    local stim = eowu:Stimulus('first' .. i)
    stim:Units('normalized')
    stim:Color(math.random(), math.random(), math.random())
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

State.Render = {
  state1 = state1_render,
  state2 = state2_render,
  state3 = state3_render
}

States = {
  State = State,
  State2 = State2,
  State3 = State3
}