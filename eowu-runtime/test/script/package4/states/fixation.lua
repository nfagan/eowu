local state = {}

state.Name = 'fixation'
state.Duration = 2000
state.Variables = {
  fixation_acquired = 0/0
}
state.First = true

function choice1()
  print('entered from choice')
end

function state.Entry()
  local script = eowu.script()
  local state = script:State('fixation')

  print(state.name)

  state:Next('images')
  script:Render('fixation')

  local targ = script:Target('first')
  
  targ:Entry(function()
    local script = eowu.script()
    script:Render('fixation')
  end)

  targ:Exit(function()
    local script = eowu.script()
    script:Render('another')
  end)

  state:Variable('fixation_acquired'):Reset()
end

function state.Exit()
  local targ = eowu.script():Target('first')
  targ:Reset()
end

local function another()
  local script = eowu.script()
  local stim = script:Stimulus('sq')

  stim:Color({1, 1, 1})

  local pos = stim.position
  pos.x = pos.x - 0.001
  stim.position = pos
  
  stim:Draw()
end

local function render()
  local script = eowu.script()
  local stim = script:Stimulus('sq')
  local renderer = script:Renderer()

  for i = 1, 100 do
    local s = script:Stimulus('sq' .. i)
    s:Units('normalized')
    s:Position({math.random(), math.random()})
    s:Size({0.01, 0.01})
    s:Color({0, 1, 0})
    s:Draw()
  end

  local pos = stim.position
  pos.x = pos.x - 0.001
  stim.position = pos

  stim:Draw()
end

state.Render = {
  fixation = render,
  another = another
}

return state