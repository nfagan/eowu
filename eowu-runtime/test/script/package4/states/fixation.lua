local state = {}

state.Name = 'fixation'
state.Duration = 2000
state.Variables = {
  fixation_acquired = 0/0
}
state.First = true

function state.Entry()
  local script = eowu.script()
  local state = script:State('fixation')

  print(state.name)

  state:Next('images')
  script:Render('fixation')

  state:Variable('fixation_acquired'):Reset()
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
  fixation = render
}

return state